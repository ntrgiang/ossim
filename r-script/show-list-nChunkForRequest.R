setwd("/nix/workspace/omnet422-inet118/so/simulations/results/")
#setwd("/home/darmstadt/nguyeng/sim/omnetpp/so/simulations/results")

require(omnetpp)

vectorFiles <- scalarFiles <- file.path('Donet_Homo_oneRouter_Network-0.vec')

dataset <- loadDataset(vectorFiles, add(select='module("Donet_Homo_oneRouter_Network.peerNode[8].peerApp.mesh") AND name("newChunkForRequest:vector")'))

data <- loadVectors(dataset, NULL)

# print(data$vectors[,c('module','name')])

l_newChunkForRequest <- data$vectordata$y

average <- mean(l_newChunkForRequest)

print(l_newChunkForRequest)
print(average)




#setwd("/home/darmstadt/nguyeng/sim/omnetpp/so/r-script/")