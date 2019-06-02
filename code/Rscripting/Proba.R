
getDataVector <- function(){
  returnVec = list()
  j = 1
  for(i in 0:117)
  {
    a <-paste("../data/results/sporuls", i, sep="") 
    tmpVec <- read.csv(a, header=TRUE, sep=";")
    sporuls <-tmpVec$sporuls
    returnVec[[j]] <-  sporuls
    j = j+1
    
  }
   return(returnVec)
}

getProbaVector <- function(dataVector){
 # seuils <- c(10^8, 5*10^7, 10^7, 5*10^6, 10^6, 5*10^5, 10^5, 5*10^4, 10^4)
  
 # par(mfrow=c(2,2))
  #for(i in 1:length(seuils)){
  #tmpseuils <- seuils[i]
  
  probaVec = c()
  total <- 0
  seuil <- 2*10^6
  for(i in 1:length(dataVector))
  {
    total = 0
    for (j in 1:length(dataVector[[i]])) {
      if(dataVector[[i]][j] >= seuil)
      {
        if(i>2)
        {
          if(dataVector[[i-1]][j] >= seuil && dataVector[[i-2]][j] >= seuil)
          {
            total <- total+1
          }
        }
      
      
      }
    }
    probaVec[i]<- (total/length(dataVector[[i]]))*100
  }

  xaxis <- 1:length(probaVec)
  
  for(i in 1:length(probaVec) )
    print(probaVec[i])
  
  plot(xaxis, probaVec, pch=16,cex=0.4, main = "Probas")
  lines(xaxis, probaVec, col="red")
  
    

  
}

main <- function(){
  
  dataVec <- getDataVector()
  probaVec <- getProbaVector(dataVec) 
#  print(length(xaxis))
  
  
}