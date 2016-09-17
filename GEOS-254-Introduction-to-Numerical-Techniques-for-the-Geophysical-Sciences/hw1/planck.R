main <- "Planck Function Curves"
xlab <- expression(paste(Wavelength, " (cm)"))
ylab <- expression(paste(Intensity, " ",(ergs/cm^3/sec/steradian)))
col <- c("blue", "orange", "red")

planck <- function(lambda, Temp) {
  #constants
  h <- 6.6261e-27 
  c <- 2.9979e10 
  kb <- 1.3807e-16   
  
  ( 2*h*c^2 ) / ( lambda^5*( exp( (h*c)/(lambda*kb*Temp) ) - 1 ) )
}

#create the function graphs
png(file="Planck.png",width=1500,height=1000,res=100)
par(mar=c(7,7,4,2)+0.1,mgp=c(5,1,0))
curve(planck(lambda=x, Temp=10), from=0, to=1.0,main=main,xlab=xlab,ylab=ylab,col=col[1],xaxt="n",yaxt="n")
curve(planck(lambda=x, Temp=100), from=0, to=1.0,main=main,add=TRUE,xlab=xlab,ylab=ylab,col=col[2],xaxt="n",yaxt="n")
curve(planck(lambda=x, Temp=1000), from=0, to=1.0,main=main, add=TRUE,xlab=xlab,ylab=ylab,col=col[3],xaxt="n",yaxt="n")
axis(1,at=seq(0,1,by=.05),las=2)
axis(2,at=seq(0,4,by=.1),las=2)
dev.off()

png(file="Planck10.png",width=1500,height=1000,res=100)
par(mar=c(7,7,4,2)+0.1,mgp=c(5,1,0))
curve(planck(lambda=x, Temp=10), from=0, to=0.1,main="Planck Function (T=10 K)",xlab=xlab,ylab=ylab,col=col[1],xaxt="n",yaxt="n")
axis(1,at=seq(0,0.1,by=.005),las=2)
axis(2,at=seq(0,5,by=.1),las=2)
dev.off()

png(file="Planck100.png",width=1500,height=1000,res=100)
par(mar=c(7,7,4,2)+0.1,mgp=c(5,1,0))
curve(planck(lambda=x, Temp=100), from=0, to=0.01,main="Planck Function (T=100 K)",xlab=xlab,ylab=ylab,col=col[2],xaxt="n",yaxt="n")
axis(1,at=seq(0,0.01,by=.0005),las=2)
axis(2,at=seq(0,5e5,by=0.5e5),las=2)
dev.off()

png(file="Planck1000.png",width=1500,height=1000,res=100)
par(mar=c(7,7,4,2)+0.1,mgp=c(5,1,0))
curve(planck(lambda=x, Temp=1000), from=0, to=0.001,main="Planck Function (T=1000 K)",xlab=xlab,ylab=ylab,col=col[3],xaxt="n",yaxt="n")
axis(1,at=seq(0,0.001,by=.00005),las=2)
axis(2,at=seq(0,5e10,by=0.5e10),las=2)
dev.off()

