# Pour MEPS 2018 - illustre clustering
rm(list=ls(all=T))
graphics.off()
repFigure="/home/poisson/MESP"
repData="/home/poisson/DYPHYMA/"
setwd(repFigure)

# Calcule matrice de similarité
#---------------------------------------------------------------------
#' Recherche du nb de cluster par selon le crit?re du gap 
#' Author Emilie Poisson Caillault v13/10/2015
#' @param val #valeur propre d'une matrice de similarite
#' @param kmax
#' @return K
compute.nbCluster.gap<-function(val,seuil=0,fig=FALSE){
  val.accepted=val[val>seuil];
  nbVal=length(val);
  gap=abs(diff(val.accepted))
  if(fig) {x11(); plot(gap);}
  K=which.max(gap);
}

#---------------------------------------------------------------------
#' Calcule matrice de similarité gaussienn
#' Author Emilie Poisson Caillault v13/10/2015
#' @param points matrice pointsxattributs 
#' @param sigma
#' @return mat
compute.similarity.gaussien<-function(points, sigma){
  e <- dist(points)
  E <- as.matrix(e)
  mat <- exp(-E^2/(2*sigma^2))
  return(mat)
}

#---------------------------------------------------------------------
#' Recherche du voisin n° id le plus proche
#' Author Emilie Poisson Caillault v13/10/2015
#' @param vdist vecteur de distance du point avec d'autres points
#' @param vois
#' @return id
search.neighboor<-function(vdist, vois){
  ind=rank(vdist,ties.method="first");
  out<-which(ind==vois)
}

#---------------------------------------------------------------------
#' Calcule matrice de similarité gaussienne selon Zelnik-Manor et Perona
#' Author Emilie Poisson Caillault v13/10/2015
#' @param points matrice pointsxattributs 
#' @param vois
#' @return mat
#' sigma local, attention risque matrice non semi-def positive
compute.similarity.ZP<-function(points, vois=7){
  vois=min(vois,nrow(points))
  e <- dist(points)
  E <- as.matrix(e)
  #identification du voisin n°vois pour chaque ligne (vois+1 car lui-meme insere)
  sigmaV=apply(E,1,function(x){ind <- NULL; ind<-search.neighboor(x,vois+1); out<-x[ind]+ .Machine$double.eps})
  #matrice des sigma i x sigma j
  sigma=sapply(sigmaV,function(x){x*sigmaV}) 
  mat <- exp(-E^2/sigma)
  return(mat)
}

NJW<-function(similarite, K=3){
  
  v=eigen(similarite,symmetric=TRUE)
  #test si matrice similarite quasi semi-definie positive (quasi papier de NJW)
  if(length(which(v$values<(-0.1)))) {
    print("matrice initiale non gram");
    similarite=similarite %*% t(similarite);
  }
  #mise a zero de la diagonale
  diag(similarite)=0;		
  degre=rowSums(similarite)
  ds <- degre^(-0.5)
  Ds=diag(ds)
  #construction du laplacien
  laplacien=Ds %*% similarite %*% Ds
  eig=eigen(laplacien,symmetric=TRUE)
  
  print(eig$values[1:30]);
  
  #classification spectrale 
  labelSpectral=NULL;
  
  #classification Kfixe 
  #selection des K premiers vecteurs propres
  Z=eig$vectors[,1:K];
  #projection de Z sur la sphere unite
  Zn=Z/apply(Z,MARGIN=1,FUN=function(x) norm(matrix(x),"f"));
  
  cl=kmeans(Zn,centers=K,iter.max = 100, nstart = 10);
  #cas de Hartigon avec pas assez de memoire
  if(cl$ifault==4){cl=kmeans(Zn,centers=cl$centers, algorithm="MacQueen"); }
  labelSpectral=cl$cluster
  out<-data.frame(label=labelSpectral,vp=eig$vectors,valp=eig$values);
}

HC<-function(points,K){
  d <- dist(points)
  h <- hclust(d,method = "ward.D2")
  label=cutree(h,K);
  out<-data.frame(label);
}



KM<-function(points,K){
  cl=kmeans(points,centers=K,iter.max = 100, nstart = 10);
  #cas de Hartigon avec pas assez de memoire
  if(cl$ifault==4){cl=kmeans(points,centers=cl$centers, algorithm="MacQueen"); }
  out<-data.frame(label=cl$cluster,K)
} 


#generation des donnees ou pas.
relance=FALSE;
if(relance==F){
  load(paste(repData,"IllustreClassif.Rdata",sep=""));
}else{
  x<-(runif(1500)-0.5)*4*pi
  y<-c(sin(x[1:500])*5+runif(500)*2, sin(x[501:1000])*5+runif(500)*2+8, sin(x[1001:1500])*5+runif(500)*2-8)
  serpentins <- scale(data.frame(x,y))
  trueLabel.serpentins <- c(rep(1,500),rep(2,500),rep(3,500))
    plot(serpentins,col=trueLabel.serpentins)
  
  d=data.frame(data=cbind(serpentins))
  
  # Calcul de la matrice de similarite  des centres representatifs des donnees
  similarity=compute.similarity.ZP(points=cbind(d$data.x,d$data.y), vois=7)
  clustering.SC=NJW(similarity,K=3)
  
  jpeg(file="SC_serpentins.jpg",1600,1600, units = "px", quality = 100, bg = "white")
  plot(serpentins,col=clustering.SC$label,cex=2, pch=clustering.SC$label+1)
  dev.off()
  
  jpeg(file="serpentins.jpg",1600,1600,units = "px", quality = 100, bg = "white")
  plot(serpentins,col="black",cex=2, pch="+")
  dev.off()
  
  #hierarchique
  clustering.HC<-HC(d,K=3);
  jpeg(file="HC_serpentins.jpg",1600,1600,units = "px", quality = 100, bg = "white")
  plot(serpentins,col=clustering.HC$label,cex=2, pch=clustering.HC$label+1)
  dev.off();
  
  #kmeans
  clustering.KM<-KM(d,K=3)
  jpeg(file="KM_serpentins.jpg",1600,1600,units = "px", quality = 100, bg = "white")
  plot(serpentins,col=clustering.KM$label,cex=1, pch=clustering.KM$label+1)
  dev.off();
  
  serpentins.label=data.frame(HC=clustering.HC$label,KM=clustering.KM$label,SC=clustering.SC$label)
  
  #####
  #carres
  
  simul<-runif
  x<-c(simul(500),simul(500),simul(500)+2); 
  y<-c(simul(500),simul(500)+2,simul(500)+1)
  carres<-scale(data.frame(x,y))
  trueLabel.carres<-c(rep(1,500),rep(2,500),rep(3,500))
  
  d=data.frame(data=cbind(carres))
  
  # Calcul de la matrice de similarite  des centres representatifs des donnees
  similarity=compute.similarity.ZP(points=cbind(d$data.x,d$data.y), vois=7)
  clustering.SC=NJW(similarity,K=3)
  
  jpeg(file="SC_carres.jpg",1600,1600,units = "px", quality = 100, bg = "white")
  plot(carres,col=clustering.SC$label,cex=2, pch=clustering.SC$label+1)
  dev.off()
  
  jpeg(file="carres.jpg",1600,1600,units = "px", quality = 100, bg = "white")
  plot(carres,col="black",cex=2, pch="+")
  dev.off()
  
  #hierarchique
  clustering.HC<-HC(d,K=3)
  jpeg(file="HC_carres.jpg",1600,1600,units = "px", quality = 100, bg = "white")
  plot(carres,col=clustering.HC$label,cex=2, pch=clustering.HC$label+1)
  dev.off();
  
  #kmeans
  clustering.KM<-KM(d,K=3)
  jpeg(file="KM_carres.jpg",1600,1600,units = "px", quality = 100, bg = "white")
  plot(carres,col=clustering.KM$label,cex=2, pch=clustering.KM$label+1)
  dev.off();
  
  carres.label=data.frame(HC=clustering.HC$label,KM=clustering.KM$label,SC=clustering.SC$label)
  
  # exemple 2 cercles + 1 disque
  n<-2000 ; r1<-0.3   ;   r2<-0.55   ;   r3<-0.65   ;   r4<-0.90  ;   r5<-1
  x<-(runif(n)-0.5)*2;y<-(runif(n)-0.5)*2
  keep<-which((x**2+y**2<r1**2)   |   (x**2+y**2>r2**2 & x**2+y**2<r3**2 )   |   (x**2+y**2>r4**2 & x**2+y**2<r5**2 ))
  disque.2cercles<-data.frame(x,y)[keep,]
  
  trueLabel.d2c<-1 + ( disque.2cercles$x**2+disque.2cercles$y**2<r1**2)*1 + (disque.2cercles$x**2+disque.2cercles$y**2>r2**2 & disque.2cercles$x**2+disque.2cercles$y**2<r3**2 )*2 
  plot(disque.2cercles,col=trueLabel.d2c)
  
  d=data.frame(data=cbind(disque.2cercles))
  
  # Calcul de la matrice de similarite  des centres representatifs des donnees
  similarity=compute.similarity.ZP(points=cbind(d$data.x,d$data.y), vois=7)
  clustering.SC=NJW(similarity,K=3)
  
  jpeg(file="SC_cercles.jpg",1600,1600,units = "px", quality = 100, bg = "white")
  plot(disque.2cercles,col=clustering.SC$label,cex=2, pch=clustering.SC$label+1)
  dev.off()
  
  jpeg(file="cercles.jpg",1600,1600,units = "px", quality = 100, bg = "white")
  plot(disque.2cercles,col="black",cex=2, pch="+")
  dev.off()
  
  #hierarchique
  clustering.HC<-HC(d,K=3)
  jpeg(file="HC_cercles.jpg",1600,1600,units = "px", quality = 100, bg = "white")
  plot(disque.2cercles,col=clustering.HC$label,cex=2, pch=clustering.HC$label+1)
  dev.off();
  
  #kmeans
  clustering.KM<-KM(d,K=3)
  jpeg(file="KM_cercles.jpg",1600,1600,units = "px", quality = 100, bg = "white")
  par(mar=c(5,3,2,2)+0.1)
  plot(disque.2cercles,col=clustering.KM$label,cex=2, pch=clustering.KM$label+1)
  dev.off();
  
  disque.2cercles.label=data.frame(HC=clustering.HC$label,KM=clustering.KM$label,SC=clustering.SC$label)
  
  save(disque.2cercles.label,disque.2cercles,serpentins,serpentins.label,carres,carres.label,file="IllustreClassif.Rdata");
}


assignVote<-function(labelVrai,label){
  t=table(labelVrai,label)
  n=rownames(t)
  v=apply(t,MARGIN=2,FUN=which.max)
  res=label;
  for (lab in 1:ncol(t)){
    ind=which(label==lab);
    res[ind]=v[lab]
  }
  return (res)
}

new=assignVote(carres.label$HC,carres.label$KM)
carres.label$KM=new

new=assignVote(carres.label$HC,carres.label$SC)
carres.label$SC=new

new=assignVote(disque.2cercles.label$HC,disque.2cercles.label$KM)
disque.2cercles.label$KM=new

new=assignVote(disque.2cercles.label$HC,disque.2cercles.label$SC)
disque.2cercles.label$SC=new

new=assignVote(serpentins.label$SC,serpentins.label$HC)
serpentins.label$HC=new

par(mfrow = c(3,3))
plot(carres,col=carres.label$HC,cex=2, pch=carres.label$HC+1, main="HierarchicalC")
plot(carres,col=carres.label$KM,cex=2, pch=carres.label$KM+1, main="K-Means")
plot(carres,col=carres.label$SC,cex=2, pch=carres.label$SC+1, main="SpectralC")

plot(disque.2cercles,col=disque.2cercles.label$HC,cex=2, pch=disque.2cercles.label$HC+1)
plot(disque.2cercles,col=disque.2cercles.label$KM,cex=2, pch=disque.2cercles.label$KM+1)
plot(disque.2cercles,col=disque.2cercles.label$SC,cex=2, pch=disque.2cercles.label$SC+1)

plot(serpentins,col=serpentins.label$HC,cex=2, pch=serpentins.label$HC+1)
plot(serpentins,col=serpentins.label$KM,cex=2, pch=serpentins.label$KM+1)
plot(serpentins,col=serpentins.label$SC,cex=2, pch=serpentins.label$SC+1)

library("ggplot2")
library(gridExtra)


monggplot<-function(df,label,titre=NULL,pos.legend="non",gauche=F, bas=F, ...){
  axisX <- if (bas) element_text(size=14) else element_blank()
  axisY <- if (gauche) element_text(size=14) else element_blank()
  
  p=ggplot(data=df, aes(x=x, y=y)) 
  p=  p +  geom_point(aes(shape=label,color=label),na.rm=TRUE) +
    scale_shape_manual(values=c(3, 16, 17, 18)) +
    scale_color_manual(values=c("black","cyan","blue","red"))+ 
    ggtitle(titre) +
    theme(panel.background = element_rect(fill = "white", colour = "grey50"),
          legend.position = pos.legend, 
          axis.title.x=axisX,axis.title.y=axisY);
  #axis.title.x=element_text(size=14),axis.title.y=element_text(size=14));
  return(p);
}
df=data.frame(x=carres[,1],y=carres[,2])
labela1=factor(carres.label$HC)
p.a1 =monggplot(df,labela1,titre="1 - HC (cut=3)", gauche=T)

labela2=factor(carres.label$KM)
p.a2 =monggplot(df,labela2,"2 - Kmeans (K=3)")

labela3=factor(carres.label$SC)
p.a3=monggplot(df,labela3,"3 - SC (K=3)")

df1=data.frame(x=disque.2cercles[,1],y=disque.2cercles[,2])
labelb1=factor(disque.2cercles.label$HC)
p.b1 =monggplot(df1,labelb1, gauche=T)

labelb2=factor(disque.2cercles.label$KM)
p.b2 =monggplot(df1,labelb2)

labelb3=factor(disque.2cercles.label$SC)
p.b3 =monggplot(df1,labelb3)

df2=data.frame(x=serpentins[,1],y=serpentins[,2])
labelc1=factor(serpentins.label$HC)
p.c1= monggplot(df2,labelc1, gauche=T, bas=T)

labelc2=factor(serpentins.label$KM)
p.c2 =monggplot(df2,labelc2, bas=T)

labelc3=factor(serpentins.label$SC)
p.c3= monggplot(df2,labelc3,pos.legend="bottom", bas=T)


#extract legend
#https://github.com/hadley/ggplot2/wiki/Share-a-legend-between-two-ggplot2-graphs
g_objet<-function(a.gplot, quoi="guide-box"){
  tmp <- ggplot_gtable(ggplot_build(a.gplot))
  noms <- sapply(tmp$grobs, function(x) x$name)
  leg <- grep(quoi, noms)
  legend <- tmp$grobs[[leg]]
  return(legend)}

malegende <- g_objet(p.c3)
df <- cbind(df, A.Squares=0,B.Circles=0,C.Snakes=0)
objet_virtuel_a <- ggplot(data=df, aes(x=A.Squares, y=y)) + xlab("A - Squares")+theme(axis.title.x=element_text(size=14,angle = 90));
objet_virtuel_b <- ggplot(data=df, aes(x=B.Circles, y=y)) + xlab("B - Circles")+theme(axis.title.x=element_text(size=14,angle = 90));
objet_virtuel_c <- ggplot(data=df, aes(x=C.Snakes, y=y)) + xlab("C - Snakes")+theme(axis.title.x=element_text(size=14,angle = 90));

axe_a <- g_objet(objet_virtuel_a, "axis.title.x")
#axe_a <- g_objet(objet_virtuel_a, "A - squares")
axe_b <- g_objet(objet_virtuel_b, "axis.title.x")
axe_c <- g_objet(objet_virtuel_c, "axis.title.x")

arr1 <- arrangeGrob(axe_a, p.a1, p.a2, p.a3, axe_b, p.b1, p.b2, p.b3, axe_c, p.c1, p.c2, p.c3 + theme(legend.position="none"), widths=c(1,10,10,10),nrow=3)
#arr2 <- arrangeGrob(arr1)
#pf=grid.arrange( arr2, malegende, heights=c(10,1))#, heigths=rep(4,9))
pf=grid.arrange( arr1, malegende, heights=c(10,1))#, heigths=rep(4,9))

pf

ggsave("clusters.pdf",plot=pf,dpi=900,units="cm",width = 17,bg = "transparent")
ggsave("clusters.eps",plot=pf,dpi=900,units="cm",width = 17,bg = "transparent")
ggsave("clusters.svg",plot=pf,dpi=900,units="cm",width = 17,bg = "transparent")
