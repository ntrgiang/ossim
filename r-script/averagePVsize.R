setwd("/nix/workspace/omnet422-inet118/so/simulations/results/")

require(omnetpp)

# scalarFiles <- file.path(system.file('extdata', package='omnetpp'), 'Scamp_Homo_oneRouter_Network-*.sca')
scalarFiles <- file.path('Scamp_Homo_oneRouter_Network-*.sca')

# dataset <- loadDataset(scalarFiles, add('scalar', 'module("Scamp_Homo_oneRouter_Network.globalStatisic") AND file(Scamp_Homo_oneRouter_Network-*.sca)'))
dataset <- loadDataset(scalarFiles, add('scalar', 'module("Scamp_Homo_oneRouter_Network.globalStatisic")'))

d <- makeBarChartDataset(dataset, rows=c('measurement'), columns=c('name'))







ci <- makeBarChartDataset(dataset, rows=c('measurement'), columns=c('name'), conf.int(0.95) )
par(mfrow=c(2,2))
plotBarChart(d, conf.int=ci, Legend.Display='true', Legend.Anchoring='NorthWest')

m<- d

x = c(100, 200)

m <- cbind(m, x)

plotLineChart(m, X.Axis.Title='simulation time', Y.Axis.Title='queue length', Legend.Display='true')


data <- data <- makeLineChartDataset(dataset, '{configname}/{runnumber} {module}')



# extracting the vector data
# vectorFiles <- system.file('extdata', 'Scamp_Homo_oneRouter_Network-0.vec', package='omnetpp')
setwd("/home/giang/workspace/422-omnet-inet-donet/scamp/simulations/results/")
require(omnetpp)
vectorFiles <- scalarFiles <- file.path('Scamp_Homo_oneRouter_Network-0.vec')
dataset <- loadDataset(vectorFiles, add(select='module("Scamp_Homo_oneRouter_Network.globalStatisic") AND name("allFinalPVsizes")'))
data <- loadVectors(dataset, NULL)
# print(data$vectors[,c('module','name')])
series <- data$vectordata$y
hist(series, seq(0, max(series), step=1))
mean(series)
