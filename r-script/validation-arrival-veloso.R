setwd("/home/giang/workspace/422-omnet-inet-donet/scamp/simulations/results/")

require(omnetpp)

vectorFiles <- scalarFiles <- file.path('validate_veloso_churn/Donet_Homo_oneRouter_Network-0.vec')

dataset <- loadDataset(vectorFiles, add(select='module("Donet_Homo_oneRouter_Network.churnModerator") AND name("arrivalTime:vector")'))

data <- loadVectors(dataset, NULL)

# print(data$vectors[,c('module','name')])

series <- data$vectordata$y

iat <- vector(mode = "numeric", length = length(series)-1)

for (i in 1:length(series)-1)
{
  iat[i] <- series[i+1] - series[i]
}

xfit <- seq(from=min(iat), to=max(iat), by=0.04)
# yfit <- dpareto(xfit, 2.52, 1.55)
yfit <- dpareto(xfit, 1.55, 2.52)
     
hist(iat, breaks=200, freq=FALSE, xlim=c(0, max(iat)/2), plot=TRUE, xlab='Inter-arrival times')
lines(xfit, yfit, col="red")

# CDF
plot.ecdf(iat, xlab="Inter-arrival times")
