library(deSolve)

predprey<- function(t, y, parms) {
    with(as.list(c(y,parms)), {
         drdt <- 2*r0 - (2*alpha*r0*f0)
         dfdt <- -f0 + (0.5*alpha*r0*f0)
         return(list(c(drdt, dfdt)))
    })
}
parms <- c(alpha=0.0)
Tmax = 40 # time horizon  
TimeStep = 0.1 # integration time step
Time <- seq(0, Tmax, by = TimeStep)
LV.out <- ode(c(r0 = 10, f0 = 200), Time, predprey,parms)
par(las=2,bty="l")
matplot(LV.out[,1],LV.out[,-1], type='l', xlab="time", ylab="Population/Density")
legend("topright",legend=c("rabbits", "foxes"), lty=c(1,2), col=c(1,2),bty="o",bg="grey",box.lty=1,pt.cex=1.0,cex=0.55)
title(main=list("h=0.1, a=0.05, R=10, F=200",font=3))

LV2.out <- ode(c(r0 = 200, f0 = 10), Time, predprey,parms)
par(las=2,bty="l")
matplot(LV2.out[,1],LV2.out[,-1], type='l', xlab="time", ylab="Population/Density")
legend("topright",legend=c("rabbits", "foxes"), lty=c(1,2), col=c(1,2),bty="o",bg="grey",box.lty=1,pt.cex=1.0,cex=0.55)
title(main=list("h=0.1, a=0.05, R=200, F=10",font=3))

Time2 <- seq(0, 40, by = 1.0)
LV3.out <- ode(c(r0 = 200, f0 = 10), Time2, predprey,parms)
par(las=2,bty="l")
matplot(LV3.out[,1],LV3.out[,-1], type='l', xlab="time", ylab="Population/Density")
legend("topright",legend=c("rabbits", "foxes"), lty=c(1,2), col=c(1,2),bty="o",bg="grey",box.lty=1,pt.cex=1.0,cex=0.55)
title(main=list("h=1.0, a=0.05, R=200, F=10",font=3))


Time3 <- seq(0, 40, by = 0.01)
LV4.out <- ode(c(r0 = 200, f0 = 10), Time3, predprey,parms)
par(las=2,bty="l")
matplot(LV4.out[,1],LV4.out[,-1], type='l', xlab="time", ylab="Population/Density")
legend("topright",legend=c("rabbits", "foxes"), lty=c(1,2), col=c(1,2),bty="o",bg="grey",box.lty=1,pt.cex=1.0,cex=0.55)
title(main=list("h=0.01, a=0.05, R=200, F=10",font=3))

parms2 <- c(alpha=0.05)
Time4 <- seq(0, 40, by = 0.1)
LV5.out <- ode(c(r0 = 100, f0 = 100), Time4, predprey,parms2)
par(las=2,bty="l")
matplot(LV5.out[,1],LV5.out[,-1], type='l', xlab="time", ylab="Population/Density")
legend("topright",legend=c("rabbits", "foxes"), lty=c(1,2), col=c(1,2),bty="o",bg="grey",box.lty=1,pt.cex=1.0,cex=0.55)
title(main=list("h=0.01, a=0.05, R=100, F=100",font=3))

parms3 <- c(alpha=0.005)
Time5 <- seq(0, 40, by = 0.1)
LV6.out <- ode(c(r0 = 100, f0 = 100), Time5, predprey,parms3)
par(las=2,bty="l")
matplot(LV6.out[,1],LV6.out[,-1], type='l', xlab="time", ylab="Population/Density")
legend("topright",legend=c("rabbits", "foxes"), lty=c(1,2), col=c(1,2),bty="o",bg="grey",box.lty=1,pt.cex=1.0,cex=0.55)
title(main=list("h=0.01, a=0.005, R=100, F=100",font=3))

