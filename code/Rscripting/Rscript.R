#install.packages("plyr")



#View(dataf)




plotData <- function(vec, n){
 print("length")  
 print(length(vec))
  #n=100

  X = (max(vec)-min(vec))/n
  print("Step: ")
  print(X)
  for (i in 1:10) {
    if (!i %% 2){
      next
    }
  }
  
  j <- min(vec) + X
  
  xaxis = c(min(vec))
  for (i in 2:n){
    xaxis[i] <- j
    j = j+ X
  }
  print("length axis1")
  print(xaxis)
  print(length(xaxis))
  print("endlength1")
  append(xaxis, max(vec))
  print("length axis2")
  print(xaxis)
  print(length(xaxis))
  print("endlength2")

  yaxis = c()
  for(i in 1:n) { yaxis[i] <- 0 }
  print("length yaxis")
  print(yaxis)
  print(length(yaxis))
  print("endlength")
  
  for (i in 1:length(yaxis)) { 
    for(j in 1:length(vec)){
      if((vec[j] < xaxis[i]+X/2) && (vec[j] >= xaxis[i]-X/2)){
        yaxis[i] <- yaxis[i]+1
      }
    }
  }
  k <- 0
  i <- 0
  for(i in 1:length(yaxis)){
    k <- k+yaxis[i]
  }
  print(k)
  plot(xaxis, yaxis, pch=16,cex=0.4)
  lines(xaxis, yaxis, col="red")
 

}


main <- function(){
  dataf<-read.csv("../data/results/sporuls99", header=TRUE, sep=";")
  library(plyr)
  vec <-as.vector(dataf$sporuls)
  vecH <- as.vector(dataf$seuilHR)
  #print(vecH)
  vecT <- as.vector(dataf$seuilTmoy)
  cnt <- count(vec)
  print(summary(vec))
  plotData(vec, 50)
 # print(length(vec))
 # plotData(vecT, 100)
  #plotData(vecH, 100)
}

#mean(vec)
#median(vec)
#quantile(vec)
#var(vec)
#sd(vec)
#summary(vec)

#plot(vec,cnt)

#par(mfrow=c(2, 2))

#boxplot(dataf$sporuls, horizontal=TRUE, main="Boxplot des Sporuls")
#boxplot(dataf$seuilHR, horizontal=TRUE, main="Boxplot des HR")
#boxplot(dataf$seuilTmoy, horizontal=TRUE, main="Boxplot des Tmoy")

#count(dataf, vars = dataf$sporuls, wt_var = NULL)

#d <- density(dataf$sporuls)
#hist(dataf$sporuls, col="red")
#plot(count(d), type="n", main="Sporuls")
#polygon(d, col="red", border="gray")

