#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reply.h"
#include "channels.h"
#include "structs.h"
#include "functions.h"
#include "handlers.h"
#include "list.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>


/* 		The strategy for the I/O of handlers:

take in message that is complete & split into three parts
	* the parts will be split into a command, a value and a message
	* all three will be stored in the cmdValueStruct, typedef'd to cmdValueStruct
		- so just leave off the "struct" when declaring an instance
also input connection information, specifically three items:
	* hostname, that is, where the server main is
	* clientIP, the ipaddress of the target client 
		- may or may not be used, depending on type of message
	* clientSocket, where the client is connected
when a response is needed, for example for RPL_WELCOME,
	* info from USER and NICK is passed into a USERstruct
		- use messageInfo struct to pass data internally
	* when both USER and NICK are decided, reply_to_one recieves
		- a messageInfoStruct with NICK and USER, and
		- a connectionStruct with connection info
every handler will pass off its data to a function, and then will return an integer
	* positive if successful (maybe zero?)
	* negative if failure

*/

int handle_NICK(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
	char *newNick = (cmdValueInstance.value ? cmdValueInstance.value : "");
	printf("handling nick:%s\n",newNick);
	if (!strcmp(newNick,""))
	{
		messageInfoStruct messageInfo = {"431","* :No nickname given"};
		int returnval = reply_to_one(messageInfo, userInfo);
		return returnval;
	}

	// If the nickname is present already
	if (is_nick_present(userList, newNick))
	{
		printf("nickname in use!\n");
		char msg[128];
		snprintf(msg, 128, "%s :Nickname is already in use",newNick);

		messageInfoStruct messageInfo = {ERR_NICKNAMEINUSE, msg};
		int returnval = reply_to_one(messageInfo, userInfo);
		return returnval;
	}


	// if the user is changing their nickname,,,
	if(userInfo->nick != "")
	{
		char msg[128];
  		snprintf(msg, 128, ":%s!%s@%s NICK :%s",userInfo->nick, userInfo->username, userInfo->clientIP, newNick);
		messageInfoStruct messageInfo = {"NICK", msg};
		int returnval = reply_to_all(messageInfo, userInfo, userInfo->userChannels);
	}

	// because userInfo is passed in, we know that the socket is connected to a user and that the user wants a new nick.
	userInfo->nick = strdup(newNick);

	// If they haven't been welcomed
	//							and if the user's nick isn't blank
	//														and if the user's username isn't blank
	if ((userInfo->welcomed) == 0 && strcmp((userInfo->nick),"") && strcmp((userInfo->username),"") )
	{
		//greet the user with the 4 required messages (done in user_connected)
		int returnval = handle_internal_WELCOME(cmdValueInstance, userInfo, userList);
		return returnval;
	}

	return 0;
}

int handle_USER(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
	char *newUser = (cmdValueInstance.value ? cmdValueInstance.value : "");
	char *fullName = (cmdValueInstance.message ? cmdValueInstance.message : "");
	int welcomed = userInfo->welcomed;
	printf("handling USER: %s\n",newUser);

	if (!strcmp(newUser,""))
	{
		messageInfoStruct messageInfo = {ERR_NEEDMOREPARAMS,"* USER :Not enough parameters"};
		int returnval = reply_to_one(messageInfo, userInfo);
		return returnval;
	}


	// What does the * * : mean? I have no idea. but it's necessary. 
	//begin full name requirement
 
	char *saveptr;
	char* stars = "* * ";
  	char* tryStars = strtok_r(fullName, ":", &saveptr);
	char* trueFullName = strtok_r(NULL, "", &saveptr);
  	// if the fullname is not given
  	//						or if try stars != "* * "
	if(!strcmp(fullName,"") || strcmp(tryStars, stars))
	{
		messageInfoStruct messageInfo = {ERR_NEEDMOREPARAMS,"USER :Not enough parameters"};
		int returnval = reply_to_one(messageInfo, userInfo);
		return returnval;
	}

	userInfo->fullName = strdup(trueFullName);


	// end fullname requirement 


	printf("handling user:%s\n",newUser);

	
	// if the username is present already, or if the user already has a username
	if (is_username_present(userList, newUser) || strcmp(userInfo->username,""))
	{
		messageInfoStruct messageInfo = {ERR_ALREADYREGISTRED,"Unauthorized command (already registered)"};
		int returnval = reply_to_one(messageInfo, userInfo);
		return returnval;
	}
	// because userInfo is passed in, we know that the socket is connected to a user.
	// by the above if, it would be impossible to change your username using this (you would already have returned).
	userInfo->username = strdup(newUser);

	// If the user hasn't seen the welcome message and both their nick and username are not \0, welcome them.
	if (welcomed == 0 && strcmp(userInfo->nick, "") && strcmp(userInfo->username,"") )
	{
		int returnval = handle_internal_WELCOME(cmdValueInstance, userInfo, userList);
		return returnval;
	}

	return 0;
}

// what to send once NICK and USER have been recieved
// This used to be in functions, but it really is more like a handler that is called by our own code.
// Therefore it will live here, but won't be included in the dispatch table.
int handle_internal_WELCOME(cmdValueStruct cmdValueInstance,user* userInfo,list* userList)
{
  char *clientIP = userInfo->clientIP;
  char *hostname = userInfo->hostname;
  char *nick = userInfo->nick;
  char *username = userInfo->username;
  char* version = "chirc-6.6.6"; //user defined?

  time_t mytime;
  struct tm *timeinfo;

  time(&mytime);
  timeinfo = localtime(&mytime);

  char* server_time = asctime(timeinfo);

  
  // RPL_WELCOME
  char msg1[128];
  snprintf(msg1, 128, ":Welcome to the Internet Relay Network %s!%s@%s",nick,username,clientIP);
  messageInfoStruct messageInfo1 = {RPL_WELCOME, msg1};
  int returnval = reply_to_one(messageInfo1,userInfo);

  // RPL_YOURHOST
  char msg2[128];
  snprintf(msg2, 128, ":Your host is %s, running version %s",hostname,version);
  messageInfoStruct messageInfo2 = {RPL_YOURHOST, msg2};
  returnval += reply_to_one(messageInfo2,userInfo);

  //RPL_CREATED
  char msg3[128];
  snprintf(msg3, 128, ":This server was created %s",server_time);
  messageInfoStruct messageInfo3 = {RPL_CREATED, msg3};
  returnval += reply_to_one(messageInfo3,userInfo);
  
  //RPL_MYINFO
  char msg4[128];
  snprintf(msg4, 128, "%s %s ao mtov",hostname,version);
  messageInfoStruct messageInfo4 = {RPL_MYINFO, msg4};
  returnval += reply_to_one(messageInfo4,userInfo);

  userInfo->welcomed = 1;

  returnval += handle_LUSERS(cmdValueInstance,userInfo,userList);
  returnval += handle_MOTD(cmdValueInstance,userInfo,userList);

  return returnval;

}

int handle_QUIT(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
	printf("handling QUIT from: %s",userInfo->nick);
	char *clientMsg = (cmdValueInstance.message ? cmdValueInstance.message : "");
	char *value = (cmdValueInstance.value ? cmdValueInstance.value : "Client Quit");

	char msg[512];
  	snprintf(msg, 512, ":%s!%s@%s QUIT %s%s%s%s",userInfo->nick,userInfo->username,userInfo->clientIP,(value[0]==':' ? "" : ":"),value,(clientMsg ? " " : ""),clientMsg);

	//char msg[128];
  	//snprintf(msg, 128, "Closing Link: %s (%s %s)",hostname,value,clientMsg);

	messageInfoStruct messageInfo = {"QUIT", msg};
	int returnval = reply_to_one(messageInfo, userInfo);
	returnval += reply_to_all(messageInfo, userInfo, userInfo->userChannels);


	leave_all_channels(userInfo->userChannels, userInfo);
	leave_all_channels(global_channel_clist, userInfo);

	remove_from_list(userList, userInfo);

	close(userInfo->clientSocket);
	pthread_exit(NULL);

	return returnval;

}


int handle_PRIVMSG(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
	char *targetNick = cmdValueInstance.value;
	printf("handling PRIVMSG from %s to %s\n",userInfo->nick, targetNick);
	char *message = cmdValueInstance.message;

	int returnval = 0;
	channel* targetChannel = NULL;
	char *targetCname = strdup(targetNick);
	user* targetUser = NULL;


	char msg[1023];
	messageInfoStruct messageInfo = {"", ""};

	// if they're sending the message to a channel
	if (targetNick[0] == '#' || targetNick[0] == '&' || targetNick[0] == '!' || targetNick[0] == '+')
	{
		channel *targetUserChannel = fetch_channel_by_cname(userInfo->userChannels, targetCname);
		channel *targetGlobalChannel = fetch_channel_by_cname(global_channel_clist, targetCname);


		// If there is no channel wwith the given cName
		if (!is_cname_present(global_channel_clist, targetCname))
		{
			snprintf(msg, 1023, "%s :No such nick/channel",targetCname);
			messageInfo.messageType = ERR_NOSUCHNICK;
			messageInfo.messageContents = msg;
			targetUser = userInfo;
			returnval += reply_to_one(messageInfo,targetUser);
		}
		// If the user is trying to send a message to a channel they're not connected to
		else if ((!is_cname_present(userInfo->userChannels, targetCname)) || (!is_char_present(targetUserChannel->umode,'v') && is_char_present(targetGlobalChannel->cmode,'m') && (!is_char_present(targetUserChannel->umode,'o') && userInfo->mode !='o')))
		{
			snprintf(msg, 1023, "%s :Cannot send to channel",targetCname);
			messageInfo.messageType = ERR_CANNOTSENDTOCHAN;
			messageInfo.messageContents = msg;
			targetUser = userInfo;
			returnval += reply_to_one(messageInfo,targetUser);
		}
		else
		{
			char hostname[128];
	    	snprintf(hostname, 128, ":%s!%s@%s",userInfo->nick,userInfo->username,userInfo->clientIP);
			targetChannel = fetch_channel_by_cname(userInfo->userChannels, targetCname);
			snprintf(msg, 1023, "%s %s %s %s",hostname, "PRIVMSG", targetCname, message);
			messageInfo.messageType = "PRIVMSG";
			messageInfo.messageContents = msg;
			returnval += reply_to_channel(messageInfo,userInfo,targetChannel);
		}
		return returnval;
	}
	else
	{
		// If there are no users or if the username is not present
		if (is_empty(userList) || !(is_nick_present(userList, targetNick)))
		{
			snprintf(msg, 1023, "%s :No such nick/channel",targetNick);
			messageInfo.messageType = ERR_NOSUCHNICK;
			messageInfo.messageContents = msg;
			targetUser = userInfo;
		}

		else
		{
			char hostname[128];
	    	snprintf(hostname, 128, ":%s!%s@%s",userInfo->nick,userInfo->username,userInfo->clientIP);
			targetUser = fetch_user_by_nick(userList, targetNick);
			snprintf(msg, 1023, "%s %s %s %s",hostname, "PRIVMSG", targetNick, message);
			messageInfo.messageType = "PRIVMSG";
			messageInfo.messageContents = msg;
		}
		
		int returnval = reply_to_one(messageInfo, targetUser);

		// RPL_AWAY
		if(targetUser->mode == 'a') {
		  char msg2[1023];
		  snprintf(msg2, 1023, "%s %s", targetUser->nick, targetUser->away_message);
		  messageInfo.messageType = RPL_AWAY;
		  messageInfo.messageContents = msg2;
		  returnval += reply_to_one(messageInfo, userInfo);
		}
		
		return returnval;
	}

}

int handle_NOTICE(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
	printf("handling NOTICE from %s\n",userInfo->nick);
	// I think this is the functionality of NOTICE... there was very little documentation on how it should work
	char *targetNick = cmdValueInstance.value;
	char *message = cmdValueInstance.message;

	int returnval = 0;
	channel* targetChannel = NULL;
	char *targetCname = strdup(targetNick);
	user* targetUser = NULL;


	char msg[1023];
	messageInfoStruct messageInfo = {"", ""};
	if (targetNick[0] == '#' || targetNick[0] == '&' || targetNick[0] == '!' || targetNick[0] == '+')
	{
		if (!is_cname_present(global_channel_clist, targetCname) || !is_cname_present(userInfo->userChannels, targetCname))
			return 1;
		else
		{
			char hostname[128];
			snprintf(hostname, 128, ":%s!%s@%s",userInfo->nick,userInfo->username,userInfo->clientIP);
			targetChannel = fetch_channel_by_cname(userInfo->userChannels, targetCname);
			snprintf(msg, 1023, "%s %s %s %s",hostname, "NOTICE", targetCname, message);
			messageInfo.messageType = "NOTICE";
			messageInfo.messageContents = msg;
			returnval += reply_to_channel(messageInfo,userInfo,targetChannel);
		}
		return returnval;
	}
	else
	{

		if (is_empty(userList) || !(is_nick_present(userList, targetNick)))
		{
			return 1;
		}
		else
		{
			char hostname[128];
			snprintf(hostname, 128, ":%s!%s@%s",userInfo->nick,userInfo->username,userInfo->clientIP);
			targetUser = fetch_user_by_nick(userList, targetNick);
			snprintf(msg, 1023, "%s %s %s %s",hostname, "NOTICE", targetNick, message);
			messageInfo.messageType = "NOTICE";
			messageInfo.messageContents = msg;
		}
		returnval += reply_to_one(messageInfo, targetUser);
		return returnval;
	}
}

int handle_PING(cmdValueStruct cmdValueInstance, user* userInfo, list* l)
{
	char msg[128];
	snprintf(msg,128,"PONG %s",userInfo->hostname);
	int clientSocket = userInfo->clientSocket;
	send(clientSocket, msg, strlen(msg), 0);

	return 1;
}
int handle_PONG(cmdValueStruct cmdValueInstance, user* userInfo, list* l)
{
	printf("pong received");
	return 1;
}

int handle_MOTD(cmdValueStruct cmdValueInstance, user* userInfo, list* l)
{
	char msg[1023];
	messageInfoStruct messageInfoh = {"",""};	//header
	messageInfoStruct messageInfo = {"",""};
	messageInfoStruct messageInfof = {"",""};	//footer
	
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen("motd.txt", "r");
	if (fp == NULL)
	{
		snprintf(msg, 1023, ":MOTD File is missing");
		messageInfo.messageType = ERR_NOMOTD;
		messageInfo.messageContents = msg;
		reply_to_one(messageInfo,userInfo);
		return 1;
	}

	else
	{
		char header[128];
		snprintf(header,128,":- %s Message of the day - ",userInfo->hostname);
		messageInfoh.messageType = RPL_MOTDSTART;
		messageInfoh.messageContents = header;
		reply_to_one(messageInfoh,userInfo);

		while ((read = getline(&line, &len, fp)) != -1) {
			snprintf(msg, 512, ":- %s",line);
			messageInfo.messageType = RPL_MOTD;
			messageInfo.messageContents = msg;
			reply_to_one(messageInfo,userInfo);
		}

		char footer[128];
		snprintf(footer,128,":End of MOTD command");
		messageInfof.messageType = RPL_ENDOFMOTD;
		messageInfof.messageContents = footer;
		reply_to_one(messageInfof,userInfo);

		if (line)
	        free(line);
	    return 1;
	}

}

int handle_LUSERS(cmdValueStruct cmdValueInstance, user* userInfo, list* l)
{
	int knownUsers = 0;
	int unknownUsers = 0;
	int services = 0;
	int servers = 1;
	int channels = 0;

	node *iter = l->head;

	// if empty list, userstats stay at 0. this shouldn't happen ever though.

	// iterate through list to find the user location
	while (iter != NULL) 
	{
		// welcomed is the attribute of choice for this function - if the user is fully registered, 
		// with a username and nickname, welcomed is automatically tripped to 1. If not, it's defaulted to 0.	
		if (iter->user->welcomed == 0) 
			unknownUsers += 1;
		else if (iter->user->welcomed == 1)
			knownUsers += 1;
		iter = iter->next;
	}

	// RPL_LUSERCLIENT
	char msg1[128];
	messageInfoStruct messageInfo1 = {"", ""};
	snprintf(msg1, 128, ":There are %d users and %d services on %d servers",knownUsers,services,servers);
	messageInfo1.messageType = RPL_LUSERCLIENT;
	messageInfo1.messageContents = msg1;
	int returnval = reply_to_one(messageInfo1, userInfo);

	// RPL_LUSEROP
	char msg2[128];
	messageInfoStruct messageInfo2 = {"", ""};
	snprintf(msg2, 128, "0 :operator(s) online");
	messageInfo2.messageType = RPL_LUSEROP;
	messageInfo2.messageContents = msg2;
	returnval += reply_to_one(messageInfo2, userInfo);

	// RPL_LUSERUNKNOWN
	char msg3[128];
	messageInfoStruct messageInfo3 = {"", ""};
	snprintf(msg3, 128, "%d :unknown connection(s)",unknownUsers);
	messageInfo3.messageType = RPL_LUSERUNKNOWN;
	messageInfo3.messageContents = msg3;
	returnval += reply_to_one(messageInfo3, userInfo);

	//RPL_LUSERCHANNELS
	char msg4[128];
	messageInfoStruct messageInfo4 = {"", ""};
	snprintf(msg4, 128, "%d :channels formed",channels);
	messageInfo4.messageType = RPL_LUSERCHANNELS;
	messageInfo4.messageContents = msg4;
	returnval += reply_to_one(messageInfo4, userInfo);

	//RPL_LUSERME
	char msg5[128];
	messageInfoStruct messageInfo5 = {"", ""};
	snprintf(msg5, 128, ":I have %d clients and %d servers",(knownUsers + unknownUsers), servers);
	messageInfo5.messageType = RPL_LUSERME;
	messageInfo5.messageContents = msg5;
	returnval += reply_to_one(messageInfo5, userInfo);

	return returnval;
}

int handle_WHOIS(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
	char *whoisNick = cmdValueInstance.value;

	if (!(is_nick_present(userList, whoisNick)))
	{
		char msg1[128];
		messageInfoStruct messageInfo1 = {"",""};
		snprintf(msg1, 128, "%s :No such nick/channel",whoisNick);
		messageInfo1.messageType = ERR_NOSUCHNICK;
		messageInfo1.messageContents = msg1;
		return reply_to_one(messageInfo1, userInfo);
	}
	else
	{
		user *myUser = fetch_user_by_nick(userList, whoisNick);

		// RPL_WHOISUSER
		char msg1[128];
		messageInfoStruct messageInfo1 = {"",""};
		snprintf(msg1, 128, "%s %s %s * :%s",myUser->nick,myUser->username,myUser->hostname,myUser->fullName);
		messageInfo1.messageType = RPL_WHOISUSER;
		messageInfo1.messageContents = msg1;
		int returnval = reply_to_one(messageInfo1, userInfo);

		// RPL_WHOISSERVER
		char msg2[128];
		messageInfoStruct messageInfo2 = {"",""};
		snprintf(msg2, 128, "%s %s :this server sucks and i hate it.",myUser->nick,myUser->hostname);
		messageInfo2.messageType = RPL_WHOISSERVER;
		messageInfo2.messageContents = msg2;
		returnval += reply_to_one(messageInfo2, userInfo);

		// RPL_ENDOFWHOIS
		char msg3[128];
		messageInfoStruct messageInfo3 = {"", ""};
		snprintf(msg3, 128, "%s :End of WHOIS list",myUser->nick);
		messageInfo3.messageType = RPL_ENDOFWHOIS;
		messageInfo3.messageContents = msg3;
		returnval += reply_to_one(messageInfo3, userInfo);

		return returnval;
	}
}
int handle_UNKNOWN(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
	char msg[128];
	messageInfoStruct messageInfo = {"",""};
	snprintf(msg,128,"%s :Unknown command",cmdValueInstance.cmd);
	messageInfo.messageType = ERR_UNKNOWNCOMMAND;
	messageInfo.messageContents = msg;
	return reply_to_one(messageInfo, userInfo); 
}

//*******				1c				*******//

int handle_JOIN(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
  char msg[1023];

	printf("handling JOIN from %s\n",userInfo->nick);
  
  // prob not gonna use this variable but I just feel better knowing
  // add_new_channel is returning something and I can see it
  channel *global_channel;

	char *cname = (cmdValueInstance.value ? cmdValueInstance.value : "");
	if(!strcmp(cname,""))
	{
		messageInfoStruct messageInfo = {"461","JOIN :Not enough parameters"};
		int returnval = reply_to_one(messageInfo, userInfo);
		return returnval;
	}
	int isnew= 0;
	// if channel does not exist, create it
	if(!is_cname_present(global_channel_clist, cname))
	{
	  fprintf(stderr,"adding new channel to global channel list\n");
	  isnew = 1;
	  global_channel = add_new_channel(global_channel_clist, cname);
	  //userInfo->mode = 'o';
	}


	// if the user is already in this channel, just return silently
 	if(is_cname_present(userInfo->userChannels, cname))
		return 0;

  
	// ADDING NEW CHANNEL
	// add user to global_channel_clist's user list
        channel *userChannel = fetch_channel_by_cname(global_channel_clist, cname);
        add_to_list(userChannel->userList, userInfo);


	// add new channel to user's channel list
	// and add user to that channel's user list
	channel *newChannel = add_new_channel(userInfo->userChannels, cname);
	// copy the list of users from the global channel list onto the user's own list
	userInfo->userChannels->head->channel->userList = userChannel->userList;
	userInfo->userChannels->head->channel->topic = userChannel->topic;

	//if the channel was newly created, make the user an operator
	if (isnew)
		add_mode(userInfo->userChannels->head->channel->umode, 'o');
	// JOIN MESSAGE
	fprintf(stderr, "Sending JOIN messages\n");

	char hostname[128];
	snprintf(hostname, 128, ":%s!%s@%s",userInfo->nick,userInfo->username,userInfo->clientIP);
	snprintf(msg, 1023, "%s %s %s", hostname, "JOIN", cname);
	
	messageInfoStruct messageInfo = {"", ""};
	messageInfo.messageType = "JOIN";
	messageInfo.messageContents = msg;
	int returnval = reply_to_one(messageInfo, userInfo);
	returnval += reply_to_channel(messageInfo, userInfo, userChannel);
	
	

	//RPL_TOPIC
	// only send if the channel topic is not empty
	if (!(!strcmp(newChannel->topic,"") || !strcmp(newChannel->topic,":")))
	{
		printf("RPL_TOPIC ON JOIN\n");
		char msg1[512];
		messageInfoStruct messageInfo1 = {"", ""};
		snprintf(msg1, 512, "%s %s",newChannel->cname,newChannel->topic);
		messageInfo1.messageType = RPL_TOPIC;
		messageInfo1.messageContents = msg1;
		returnval += reply_to_one(messageInfo1, userInfo);
	}
	//RPL_NAMREPLY
	char msg2[1023];
	messageInfoStruct messageInfo2 = {"", ""};
	char *usersString = fetch_users(newChannel->userList);
	snprintf(msg2, 1023, "= %s :%s",newChannel->cname, usersString);
	messageInfo2.messageType = RPL_NAMREPLY;
	messageInfo2.messageContents = msg2;
	returnval += reply_to_one(messageInfo2, userInfo);

	//RPL_ENDOFNAMES
	char msg3[128];
	messageInfoStruct messageInfo3 = {"", ""};
	snprintf(msg3, 128, "%s :End of NAMES list",newChannel->cname);
	messageInfo3.messageType = RPL_ENDOFNAMES;
	messageInfo3.messageContents = msg3;
	returnval += reply_to_one(messageInfo3, userInfo);

	return 1;
}


// this only handles one channel at the minute, but the reference server only handles one...
int handle_PART(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
	printf("handling PART from %s\n",userInfo->nick);

	channel* targetChannel = NULL;
	char msg[128];
	messageInfoStruct messageInfo = {"",""};

	char* targetCname = cmdValueInstance.value;
	char* message = cmdValueInstance.message ? cmdValueInstance.message : "";

	fprintf(stderr, "checking if the channel doesn't exist\n");
	// if the channel doesn't exist
	if (!is_cname_present(global_channel_clist,targetCname))
	{
		snprintf(msg,128,"%s :No such channel",targetCname);
		messageInfo.messageType = ERR_NOSUCHCHANNEL;
		messageInfo.messageContents = msg;
		fprintf(stderr, "sending reply ERR_NOSUCHCHANNEL\n");
		return reply_to_one(messageInfo, userInfo); 
	}
	// if the channel does exist, but the user isn't connected
	else if (!is_cname_present(userInfo->userChannels, targetCname))
	{
	  fprintf(stderr, "checked that channel existed but user wasn't connected\n");
		snprintf(msg,128,"%s :You're not on that channel",targetCname);
		messageInfo.messageType = ERR_NOTONCHANNEL;
		messageInfo.messageContents = msg;
		fprintf(stderr, "sending reply ERR_NOTONCHANNEL\n");
		return reply_to_one(messageInfo, userInfo); 
	}
	// otherwise, if they're allowed to part, do it.
	else
	{
		targetChannel = fetch_channel_by_cname(global_channel_clist, targetCname);
		if (!strcmp(message,""))
		  
		  snprintf(msg,128,":%s!%s@%s PART %s",userInfo->nick, userInfo->username, userInfo->clientIP, targetCname);
		else
		  snprintf(msg,128,":%s!%s@%s PART %s %s",userInfo->nick, userInfo->username, userInfo->clientIP, targetCname, message);
		messageInfo.messageType = "PART";
		messageInfo.messageContents = msg;
		int returnval = reply_to_channel(messageInfo, userInfo, targetChannel); 
		
		char* removing = remove_from_clist(userInfo->userChannels, targetChannel);
		remove_from_list(targetChannel->userList, userInfo);
		if (is_empty(targetChannel->userList))
		  {
		    printf("removing from global list\n");
		    remove_from_clist(global_channel_clist, targetChannel);
		  }
		
		return returnval;
	}
	return 1;
}
int handle_TOPIC(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
	printf("handling TOPIC from %s\n",userInfo->nick);

	channel* targetChannel = NULL;
	channel* targetUserChannel = NULL;

	char msg[128];
	messageInfoStruct messageInfo = {"",""};

	char* targetCname = cmdValueInstance.value;
	char* message = cmdValueInstance.message ? cmdValueInstance.message : "";

	if (!is_cname_present(userInfo->userChannels, targetCname))
	{
	  	fprintf(stderr, "checked that channel existed but user wasn't connected\n");
		snprintf(msg,128,"%s :You're not on that channel",targetCname);
		messageInfo.messageType = ERR_NOTONCHANNEL;
		messageInfo.messageContents = msg;
		fprintf(stderr, "sending reply ERR_NOTONCHANNEL\n");
		return reply_to_one(messageInfo, userInfo); 
	}

	targetChannel = fetch_channel_by_cname(global_channel_clist,targetCname);
	targetUserChannel = fetch_channel_by_cname(userInfo->userChannels,targetCname);
	char* topic = targetChannel->topic ? targetChannel->topic : "";

	// if they're changing the topic
	if (message[0]==':')
	{
		// if they're not allowed
		if(is_char_present(targetChannel->cmode,'t') && !is_char_present(targetUserChannel->umode,'o') && userInfo->mode != 'o')
		{
			snprintf(msg,128,"%s :You're not channel operator",targetCname);
			messageInfo.messageType = ERR_CHANOPRIVSNEEDED;
			messageInfo.messageContents = msg;
			fprintf(stderr, "sending reply ERR_CHANOPRIVSNEEDED\n");
			return reply_to_one(messageInfo, userInfo); 
		}
		targetChannel->topic = strdup(message);
		snprintf(msg,128,":%s!%s@%s TOPIC %s %s",userInfo->nick, userInfo->username, userInfo->clientIP, targetCname, targetChannel->topic);
		messageInfo.messageType = "TOPIC";
		messageInfo.messageContents = msg;
		return reply_to_channel(messageInfo, userInfo, targetChannel);
	}

	// if they just want to read the topic
	else
	{
		if (!strcmp(topic,"") || !strcmp(topic,":"))
		{
			snprintf(msg,128,"%s :No topic is set",targetCname);
			messageInfo.messageType = RPL_NOTOPIC;
			messageInfo.messageContents = msg;
			return reply_to_one(messageInfo, userInfo);
		}
		else
		{
			messageInfo.messageType = RPL_TOPIC;
			snprintf(msg,128,"%s %s",targetCname, targetChannel->topic);
			messageInfo.messageContents = msg;
			return reply_to_one(messageInfo, userInfo);
		}
	}
	return 1;
}
int handle_OPER(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
	printf("handling OPER from %s\n",userInfo->nick);
	char msg[128];
	messageInfoStruct messageInfo = {"",""};

	char* name = cmdValueInstance.value;
	char* trypassword = cmdValueInstance.message ? cmdValueInstance.message : "";

	if(strcmp(trypassword, userInfo->password))
	{
		snprintf(msg,128,":Password incorrect");
		messageInfo.messageType = ERR_PASSWDMISMATCH;
		messageInfo.messageContents = msg;
		return reply_to_one(messageInfo, userInfo);
	}
	userInfo->mode = 'o';
	snprintf(msg,128,":You are now an IRC operator");
	messageInfo.messageType = RPL_YOUREOPER;
	messageInfo.messageContents = msg;
	return reply_to_one(messageInfo, userInfo);
}

int handle_MODE(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{

	/*
	This is the deal. 
	OPER makes users operators on the server. this fact is stored in userInfo->mode
	channel modes (cMODE)s are stored in global_channel_clist->node->channel->cmode
		ex. moderated or not
		global_channels_clist->node->channel->umode should be empty
	user modes (uModes) are stored in userInfo->userChannels->node->channel->umode
		ex. voice privilages
		userInfo->userChannels->node->channel->cmode should be empty
	*/

	printf("handling MODE from %s\n",userInfo->nick);

	char msg[128];
	messageInfoStruct messageInfo = {"",""};
	char *target = cmdValueInstance.value;
	char *modeflags = cmdValueInstance.message ? cmdValueInstance.message : "";

	if(target[0]=='#')
	{
		if (!is_cname_present(global_channel_clist, target))
		{
			snprintf(msg,128,"%s :No such channel",target);
			messageInfo.messageType = ERR_NOSUCHCHANNEL;
			messageInfo.messageContents = msg;
			fprintf(stderr, "sending reply ERR_NOSUCHCHANNEL\n");
			return reply_to_one(messageInfo, userInfo); 
		}
		channel *targetGlobalChannel = fetch_channel_by_cname(global_channel_clist,target);
		channel *targetUserChannel = fetch_channel_by_cname(userInfo->userChannels,target);

		if((is_char_present(targetGlobalChannel->cmode,'m') && !is_char_present(targetUserChannel->umode, 'o') && userInfo->mode != 'o') || !targetUserChannel)
		{
			snprintf(msg,128,"%s :You're not channel operator",target);
			messageInfo.messageType = ERR_CHANOPRIVSNEEDED;
			messageInfo.messageContents = msg;
			fprintf(stderr, "sending reply ERR_CHANOPRIVSNEEDED\n");
			return reply_to_one(messageInfo, userInfo); 
		}
		printf("handling channel MODE %s %s\n",cmdValueInstance.value, cmdValueInstance.message);
		return handle_internal_MODE_at_channel(cmdValueInstance,userInfo,userList);
	}
	else
	{
		if (strlen(modeflags) != 2)
		{
			printf("modeflag not two chars: '%s'\n",modeflags);
			snprintf(msg,128,":unknown MODE flag");
			messageInfo.messageType = ERR_UMODEUNKNOWNFLAG;
			messageInfo.messageContents = msg;
			return reply_to_one(messageInfo, userInfo);
		}
		if (strcmp(userInfo->nick, target))
		{
			snprintf(msg,128,":Cannot change mode for other users");
			messageInfo.messageType = ERR_USERSDONTMATCH;
			messageInfo.messageContents = msg;
			fprintf(stderr, "sending reply ERR_USERSDONTMATCH\n");
			return reply_to_one(messageInfo, userInfo); 
		}
		printf("handling user MODE\n");
		return handle_internal_MODE_at_user(cmdValueInstance,userInfo,userList);
	}
	return 1;
}

int handle_internal_MODE_at_channel(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
	char msg[128];
	messageInfoStruct messageInfo = {"",""};
	char *targetCname = cmdValueInstance.value;
	char *modeflags = cmdValueInstance.message ? cmdValueInstance.message : "";
	channel *targetGlobalChannel = fetch_channel_by_cname(global_channel_clist,targetCname);
	channel *targetUserChannel = fetch_channel_by_cname(userInfo->userChannels,targetCname);

	if(!strcmp(modeflags,""))
	{
		printf("no modeflag set, sending channel modes\n");
		snprintf(msg,128,"%s +%s",targetCname, targetGlobalChannel->cmode);
		messageInfo.messageType = RPL_CHANNELMODEIS;
		messageInfo.messageContents = msg;
		int returnval = reply_to_one(messageInfo, userInfo);
		returnval += reply_to_channel(messageInfo, userInfo, targetGlobalChannel);
		return returnval;
	}
	char *saveptr;
	char *modeflagsCopy = strdup(modeflags);
	char *mode = strtok_r(modeflagsCopy, " ", &saveptr);
	char *targetNick = strtok_r(NULL,"",&saveptr);
	printf("targetNick:'%s'\n",targetNick);

	if(!is_char_present(targetUserChannel->umode,'o') && userInfo->mode != 'o')
	{
		snprintf(msg,128,"%s :You're not channel operator",targetCname);
		messageInfo.messageType = ERR_CHANOPRIVSNEEDED;
		messageInfo.messageContents = msg;
		fprintf(stderr, "sending reply ERR_CHANOPRIVSNEEDED\n");
		return reply_to_one(messageInfo, userInfo); 
	}
	//if it is a uMODE
	if (modeflags[1] == 'v' || modeflags[1] == 'o')
	{
		printf("changing user's channel mode\n");
		if(targetNick == '\0')
		{
			snprintf(msg,128,"%c :is unknown mode char to me for %s",modeflags[1],targetCname);
			messageInfo.messageType = ERR_UNKNOWNMODE;
			messageInfo.messageContents = msg;
			return reply_to_one(messageInfo, userInfo);
		}
		else if (!(is_nick_present(targetGlobalChannel->userList, targetNick)))
		{
			char msg1[128];
			messageInfoStruct messageInfo1 = {"",""};
			snprintf(msg1, 128, "%s %s :They aren't on that channel",targetNick,targetCname);
			messageInfo1.messageType = ERR_USERNOTINCHANNEL;
			messageInfo1.messageContents = msg1;
			return reply_to_one(messageInfo1, userInfo);
		}

		else
		{
			printf("user (target): %s, %s\n",mode, targetNick);
			user *targetUser = fetch_user_by_nick(userList, targetNick);
			channel *targetNickChannel =fetch_channel_by_cname(targetUser->userChannels,targetCname);
			if(modeflags[0] == '-')
			{
				remove_mode(targetNickChannel->umode,modeflags[1]);
				printf("modes:%s\n",targetNickChannel->umode);

			}
			else if(modeflags[0] == '+')
			{
				add_mode(targetNickChannel->umode,modeflags[1]);
				printf("modes:%s\n",targetNickChannel->umode);
			}
		}
		snprintf(msg,128,":%s!%s@%s MODE %s %s",userInfo->nick, userInfo->username, userInfo->clientIP,targetCname, modeflags);
		messageInfo.messageType = "MODE";
		messageInfo.messageContents = msg;
		int returnval = 0;//reply_to_one(messageInfo, userInfo);
		returnval += reply_to_channel(messageInfo, userInfo, targetGlobalChannel);
		return returnval;
	}

	//if it is a cMODE
	else if ((modeflags[1] == 'm' || modeflags[1] == 't') && !targetNick)
	{
		printf("changing global channel mode\n");
		if(modeflags[0] == '-')
		{
			printf("removing...");
			remove_mode(targetGlobalChannel->cmode, modeflags[1]);
			printf("removed\n");
		}
		else if(modeflags[0] == '+')
		{
			printf("adding\n");
			add_mode(targetGlobalChannel->cmode,modeflags[1]);
		}
	}
	else
	{
		snprintf(msg,128,"%c :is unknown mode char to me for %s",modeflags[1],targetCname);
		messageInfo.messageType = ERR_UNKNOWNMODE;
		messageInfo.messageContents = msg;
		return reply_to_one(messageInfo, userInfo);
	}

	snprintf(msg,128,":%s!%s@%s MODE %s %s",userInfo->nick, userInfo->username, userInfo->clientIP, targetCname, modeflags);
	messageInfo.messageType = "MODE";
	messageInfo.messageContents = msg;
	fprintf(stderr, "sending reply from cMODE\n");
	return reply_to_channel(messageInfo, userInfo,targetGlobalChannel); 
	return 1;
}
int handle_internal_MODE_at_user(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
	char msg[128];
	messageInfoStruct messageInfo = {"",""};
	char *modeflags = cmdValueInstance.message;

	if(!strcmp(modeflags,"+o") || modeflags[1] == 'a')
		return 0;
	else if (modeflags[1] == 'i' || modeflags[1] == 'w' || modeflags[1] == 'r' || modeflags[1] == 'o' || modeflags[1] == 'O' || modeflags[1] == 's')
	{
		printf("user %s %c->",userInfo->nick,userInfo->mode);
		
		if(modeflags[0] == '-')
			userInfo->mode = '\0';
		else if(modeflags[0] == '+')
			userInfo->mode = modeflags[1];
		
		printf("%c\n",userInfo->mode);
		
		snprintf(msg,128,":%s MODE %s :%s",userInfo->nick, userInfo->nick, modeflags);
		messageInfo.messageType = "MODE";
		messageInfo.messageContents = msg;
		return reply_to_one(messageInfo, userInfo);
	}
	else
	{
		snprintf(msg,128,":Unknown MODE flag");
		messageInfo.messageType = ERR_UMODEUNKNOWNFLAG;
		messageInfo.messageContents = msg;
		return reply_to_one(messageInfo, userInfo);
	}

	return 1;
}


int handle_AWAY(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
  
  fprintf(stderr, "handling AWAY\n");
  int returnval;

  char msg[1023];
  messageInfoStruct messageInfo = {"", ""};

  char *part1 = (cmdValueInstance.value ? strdup(cmdValueInstance.value) : "");
  char *part2 = (cmdValueInstance.message ? strdup(cmdValueInstance.message) : "");
  
  int len = strlen(part1) + strlen(part2) + 2;

  char *away_message = (char*)malloc(len*sizeof(char) +2);
  snprintf(away_message, len, "%s %s",part1,part2);  

  // RPL_UNAWAY
  if (!strcmp(part1,"") || !strcmp(part2,"")) {
    
    // set user's away message, and reset mode
    userInfo->away_message = away_message;
    userInfo->mode = '\0';

    snprintf(msg, 1023, ":You are no longer marked as being away");
    messageInfo.messageType = RPL_UNAWAY;
    messageInfo.messageContents = msg;
    returnval = reply_to_one(messageInfo, userInfo);
  }
  // RPL_NOWAWAY
  else {
    userInfo->away_message = away_message;
    userInfo->mode = 'a';

    snprintf(msg, 1023, ":You have been marked as being away");
    messageInfo.messageType = RPL_NOWAWAY;
    messageInfo.messageContents = msg;
    returnval = reply_to_one(messageInfo, userInfo);
  }

  return 1;
}
int handle_NAMES(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
  fprintf(stderr, "handling NAMES\n");
  int returnval = 0;

  char *cname = (cmdValueInstance.value ? cmdValueInstance.value : "");

  // If channel param was given
  if(strcmp(cname, "") != 0) {
    channel *targetChannel = fetch_channel_by_cname(global_channel_clist, cname);
    
    //RPL_NAMREPLY
    char msg1[1023];
    messageInfoStruct messageInfo1 = {"", ""};
    char *usersString = fetch_users(targetChannel->userList);
    snprintf(msg1, 1023, "= %s :%s",cname, usersString);
    messageInfo1.messageType = RPL_NAMREPLY;
    messageInfo1.messageContents = msg1;
    returnval = reply_to_one(messageInfo1, userInfo);

    //RPL_ENDOFNAMES
    char msg2[128];
    messageInfoStruct messageInfo2 = {"", ""};
    snprintf(msg2, 128, "%s :End of NAMES list",cname);
    messageInfo2.messageType = RPL_ENDOFNAMES;
    messageInfo2.messageContents = msg2;
    returnval += reply_to_one(messageInfo2, userInfo);
  }
  // NO PARAM GIVEN
  else {
    cnode *citer = global_channel_clist->head;

    while(citer != NULL) {
      list *userList = citer->channel->userList;
      
      //RPL_NAMREPLY
      char msg1[1023];
      messageInfoStruct messageInfo1 = {"",""};
      char *usersString = fetch_users(userList);
      snprintf(msg1, 1023, "= %s :%s",citer->channel->cname, usersString);
      messageInfo1.messageType = RPL_NAMREPLY;
      messageInfo1.messageContents = msg1;
      returnval += reply_to_one(messageInfo1, userInfo);

      citer = citer->next;
    }

    char *not_in_channel = fetch_users_notJoined(global_user_list);

    // All users JOIN'ed a channel
    if(strcmp(not_in_channel, "") == 0)
      {
	//RPL_ENDOFNAMES
	char msg2[128];
	messageInfoStruct messageInfo2 = {"", ""};
	snprintf(msg2, 128, "* :End of NAMES list");
	messageInfo2.messageType = RPL_ENDOFNAMES;
	messageInfo2.messageContents = msg2;
	returnval += reply_to_one(messageInfo2, userInfo);
      }
    else // There are un-JOINED users
      {
	//RPL_NAMREPLY
	char message[1023];
	messageInfoStruct messageInfo3 = {"", ""};
	snprintf(message, 1023, "* * :%s", not_in_channel);
	messageInfo3.messageType = RPL_NAMREPLY;
	messageInfo3.messageContents = message;
	returnval += reply_to_one(messageInfo3, userInfo);

	//RPL_ENDOFNAMES
        char msg3[128];
        messageInfoStruct messageInfo4 = {"", ""};
        snprintf(msg3, 128, "* :End of NAMES list");
        messageInfo4.messageType = RPL_ENDOFNAMES;
        messageInfo4.messageContents = msg3;
        returnval += reply_to_one(messageInfo4, userInfo);
      }
  }

  return 1;
}
int handle_LIST(cmdValueStruct cmdValueInstance, user* userInfo, list* userList)
{
  fprintf(stderr, "handling LIST\n");
  int returnval = 0;
  
  char *cname = (cmdValueInstance.value ? cmdValueInstance.value : "");
  
  // If channel param was given
  if(strcmp(cname, "") != 0) {
    channel *targetChannel = fetch_channel_by_cname(global_channel_clist, cname);
    
    //RPL_LIST
    char msg1[1023];
    messageInfoStruct messageInfo1 = {"", ""};
    int num_users = list_size(targetChannel->userList);
    char *top = targetChannel->topic;
    char *topic = top + 1;
    snprintf(msg1, 1023, ":%s %s %s %s %d :%s",userInfo->hostname, RPL_LIST, userInfo->nick, cname, list_size, topic);
    messageInfo1.messageType = RPL_LIST;
    messageInfo1.messageContents = msg1;
    returnval = reply_to_one(messageInfo1, userInfo);

    // RPL_LISTEND
    char msg2[1023];
    messageInfoStruct messageInfo2 = {"", ""};
    snprintf(msg2, 1023, ":End of LIST");
    messageInfo2.messageType = RPL_LISTEND;
    messageInfo2.messageContents = msg2;
    returnval += reply_to_one(messageInfo2, userInfo);
  }
  else { // NO PARAM GIVEN
    cnode *citer = global_channel_clist->head;

    while (citer != NULL) {
      channel *lChannel = citer->channel;
      char *name = lChannel->cname;
      int csize = list_size(lChannel->userList);
      char *topic = lChannel->topic;

      //RPL_LIST
      char message[1023];
      messageInfoStruct messageInfo = {"", ""};
      char *top = topic + 1;
      snprintf(message, 1023, ":%s %s %s %s %d :%s",userInfo->hostname, RPL_LIST, userInfo->nick, name, csize, top);
      messageInfo.messageType = RPL_LIST;
      messageInfo.messageContents = message;
      returnval = reply_to_one(messageInfo, userInfo);

      citer = citer->next;
    }
    //RPL_LISTEND
    char message2[1023];
    messageInfoStruct messageInfo3 = {"", ""};
    snprintf(message2, 1023, ":End of LIST");
    messageInfo3.messageType = RPL_LISTEND;
    messageInfo3.messageContents = message2;
    returnval += reply_to_one(messageInfo3, userInfo);
  }
  return 1;
}
int handle_WHO(cmdValueStruct cmdValueInstance, user* userInfo, list* userList){return 1;}
