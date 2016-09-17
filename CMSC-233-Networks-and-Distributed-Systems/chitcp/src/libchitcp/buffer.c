/*
 *  chiTCP - A simple, testable TCP stack
 *
 *  Functions to utilize circular buffer structure
 */

/*
 *  Copyright (c) 2013-2014, The University of Chicago
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  - Neither the name of The University of Chicago nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include "chitcp/buffer.h"

int circular_buffer_init(circular_buffer_t *buf, uint32_t maxsize)
{
  
  uint8_t *data = (uint8_t*)malloc(sizeof(uint8_t)*maxsize);
  if(!data)
    return CHITCP_ENOMEM;
  buf->data = data;
  buf->start = 0;
  buf->end = 0;
  buf->seq_initial = 0;
  buf->seq_start = 0;
  buf->seq_end = 0;
  buf->maxsize = maxsize;
  buf->count = 0;
  buf->closed = 0;

  sem_init(&buf->count_m, 0, 1);
  sem_init(&buf->mutex, 0, 1);
  sem_init(&buf->slots, 0, maxsize);
  sem_init(&buf->items, 0, 0);
  
  return CHITCP_OK;
}

int circular_buffer_set_seq_initial(circular_buffer_t *buf, uint32_t seq_initial)
{
    buf->seq_initial = seq_initial;
    buf->seq_start = seq_initial;
    buf->seq_end = seq_initial + buf->end;

    return CHITCP_OK;
}


int circular_buffer_insert(circular_buffer_t *buf, uint8_t item)
{
  if(buf->closed == 1) {
    fprintf(stderr, "in circular_buffer_insert: buffer is closed\n");
    return -1;
  }

  sem_wait(&buf->slots); // wait for available slot
  sem_wait(&buf->mutex); // lock buffer

  buf->data[buf->end] = item;

  buf->end = (buf->end + 1)%(buf->maxsize);

  sem_wait(&buf->count_m); // lock access to count
  buf->count = buf->count + 1;

  sem_post(&buf->count_m); // unlock it
  sem_post(&buf->mutex); // unlock buffer
  sem_post(&buf->items); // announce available item

  return buf->count;
}


int circular_buffer_remove(circular_buffer_t *buf)
{
  uint8_t item;
  if(buf->closed == 1) {
    fprintf(stderr, "in circular_buffer_remove: buffer is closed\n");
    return -1;
  }
  sem_wait(&buf->items);
  sem_wait(&buf->mutex);
  
  item = buf->data[buf->start];
  buf->data[buf->start] = 0;

  buf->start = (buf->start + 1)%(buf->maxsize);

  sem_wait(&buf->count_m);
  buf->count = buf->count - 1;

  sem_post(&buf->count_m);
  sem_post(&buf->mutex);
  sem_post(&buf->slots);

  return item;
}

int circular_buffer_remove_peek(circular_buffer_t *buf)
{
  uint8_t item;
  if(buf->closed == 1) {
    fprintf(stderr, "in circular_buffer_remove_peek: buffer is closed\n");
    return -1;
  }
  sem_wait(&buf->items);
  sem_wait(&buf->mutex);

  item = buf->data[buf->start];

  buf->start = (buf->start + 1)%(buf->maxsize);

  sem_post(&buf->mutex);
  sem_post(&buf->slots);

  return item;
}




int circular_buffer_write(circular_buffer_t *buf, uint8_t *data, uint32_t len, bool_t blocking)
{
  int closed;

  
  if (blocking == BUFFER_NONBLOCKING) { 
    if ((buf->maxsize - buf->count) < len) { 
      fprintf(stderr, "returning CHITCP_EWOULDBLOCK\n");
      return CHITCP_EWOULDBLOCK;
    }
    else {
      int i;
      for(i = 0; i < len; i++)
	{
	  closed = circular_buffer_insert(buf, data[i]);
	  if(closed == -1) {
            fprintf(stderr, "in circular_buffer_write: buf was closed\n");
	    return i;
	  }
	}
      buf->seq_end += len;
      return len;
    }
  }
 
  if (blocking == BUFFER_BLOCKING) {
    int i;
    for(i = 0; i < len; i++)
      {
	closed = circular_buffer_insert(buf, data[i]);

	if(closed == -1) {
	  fprintf(stderr, "in circular_buffer_write: buf was closed\n");
	  return i;
	}
      } 
  }
      
  buf->seq_end += len;

  return len;
}


int circular_buffer_read(circular_buffer_t *buf, uint8_t *dst, uint32_t len, bool_t blocking)
{
  uint32_t toread = 0;
  
  if (blocking == BUFFER_NONBLOCKING) {
    if(buf->count == 0)
      return CHITCP_EWOULDBLOCK;
    else {
      if(len < buf->count)
	toread = len;
      else
	toread = buf->count;

      int i;
      for(i = 0; i < toread; i++) {
	int item = circular_buffer_remove(buf);
	
	if(item == -1)
	  return 0;
	dst[i] = item;
      }
    }
    buf->seq_start += toread;
    return toread;
  }
  
  if(blocking == BUFFER_BLOCKING) {
    if(buf->count != 0) {
      if(len < buf->count)
	toread = len;
      else
	toread = buf->count;

      int i;
      for(i = 0; i < toread; i++) {
	int item = circular_buffer_remove(buf);
	if(item == -1)
	  return 0;
	dst[i] = item;
      }
      buf->seq_start += toread;
      return toread;
    }
    else {
      int i;
      
      uint8_t item = circular_buffer_remove(buf);
      if(item == -1)
	return 0;

      dst[0] = item;
      
      buf->seq_start += 1;
      return 1;
    }
  }
}

int circular_buffer_peek(circular_buffer_t *buf, uint8_t *dst, uint32_t len, bool_t blocking)
{
  uint32_t toread = 0;
  uint32_t start = buf->start;
  uint32_t end = buf->end;

  if (blocking == BUFFER_NONBLOCKING) {
    if(buf->count == 0)
      return CHITCP_EWOULDBLOCK;
    else {
      if(len < buf->count)
        toread = len;
      else
        toread = buf->count;

      int i;
      for(i = 0; i < toread; i++) {
        int item = circular_buffer_remove_peek(buf);

        if(item == -1) {
	  buf->start = start;
	  buf->end = end;
          return 0;
	}
        dst[i] = item;
      }
    }
    buf->start = start;
    buf->end = end;
    buf->seq_start += toread;
    return toread;
  }

  if(blocking == BUFFER_BLOCKING) {
    if(buf->count != 0) {
      if(len < buf->count)
        toread = len;
      else
        toread = buf->count;

      int i;
      for(i = 0; i < toread; i++) {
        int item = circular_buffer_remove_peek(buf);

        if(item == -1) {
	  buf->start = start;
	  buf->end = end;
          return 0;
	}
        dst[i] = item;
      }
      buf->start = start;
      buf->end = end;
      buf->seq_start += toread;
      return toread;
    }
    else {
      uint8_t item = circular_buffer_remove_peek(buf);

      if(item == -1)
        return 0;
      dst[0] = item;

      buf->start = start;
      buf->end = end;
      buf->seq_start += 1;
      return 1;
    }
  }

  buf->start = start;
  buf->end = end;
  buf->seq_start += toread;

  return toread;


}

int circular_buffer_first(circular_buffer_t *buf)
{
    return buf->seq_start;
}

int circular_buffer_next(circular_buffer_t *buf)
{
    return buf->seq_end;
}

int circular_buffer_capacity(circular_buffer_t *buf)
{
    return buf->maxsize;
}

int circular_buffer_count(circular_buffer_t *buf)
{
  return buf->count;
}

int circular_buffer_available(circular_buffer_t *buf)
{
  return buf->maxsize - buf->count;
}

int circular_buffer_dump(circular_buffer_t *buf)
{
    printf("# # # # # # # # # # # # # # # # #\n");

    printf("maxsize: %i\n", buf->maxsize);
    printf("count: %i\n", buf->count);

    printf("start: %i\n", buf->start);
    printf("end: %i\n", buf->end);
    int i;
    for(i=0; i<buf->maxsize; i++)
    {
        printf("data[%i] = %i", i, buf->data[i]);
        if(i==buf->start)
            printf("  <<< START");
        if(i==buf->end)
            printf("  <<< END");
        printf("\n");
    }
    printf(" # # # # # # # # # # # # # # # # #\n");

    return CHITCP_OK;
}

int circular_buffer_close(circular_buffer_t *buf)
{
  buf->closed = 1;

  return CHITCP_OK;
}

int circular_buffer_free(circular_buffer_t *buf)
{
  free(buf->data);
  return CHITCP_OK;
}
