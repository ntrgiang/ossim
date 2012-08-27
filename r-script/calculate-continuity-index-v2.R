# Path settings
  #setwd("/nix/workspace/omnet422-inet118/so/simulations/results/")
  #setwd("/home/darmstadt/nguyeng/sim/omnetpp/so/simulations/results/")
  setwd("/nix/workspace/omnet422-inet118/so/simulations/results/from-fuch/")

# Load library
  require(omnetpp)

# Input files
  scalarFiles <- file.path('Donet_Homo_oneRouter_Network-*.sca')

# Measured values
  dataset_hit <- loadDataset(scalarFiles, add(select='module("Donet_Homo_oneRouter_Network.globalStatistic") AND name("ChunkHit:count")'))
  dataset_miss <- loadDataset(scalarFiles, add(select='module("Donet_Homo_oneRouter_Network.globalStatistic") AND name("ChunkMiss:count")'))
  # print(dataset_Hit)
  # print(dataset_Miss)

# Parameters
  dataset_maxNOP <- loadDataset(scalarFiles, add(select='module("Donet_Homo_oneRouter_Network.peerNode[0].peerApp.mesh") AND name("maxNOP")'))
  # print(dataset_maxNOP)

#   c_runid <- dataset_hit$scalars$runid
#   df_hit <- dataset_hit$scalars[,order(dataset_hit$scalars$runid)]
#   df_miss <- dataset_miss$scalars[,order(dataset_miss$scalars$runid)]
  
    df_hit <- dataset_hit$scalars
    df_miss <- dataset_miss$scalars

    df_bind <- cbind(df_hit, df_miss$value)
    df_bind  
  
    df_ci <- transform(df_bind, value = value / (value + df_miss$value))
    df_ci
  
#  df_bind <- cbind(as.integer(dataset_hit$scalars$runid), as.numeric(dataset_hit$scalars$value), as.numeric(dataset_miss$scalars$value))
# ----------- Testing ----------------
  # -- Pretending the parameter passing
    dataset <- dataset_hit
    rows=c('measurement')
    columns=c('name')
    aggregate = 'mean'
  
  # -- Function declaration
    getRunsInWideFormat <- function (runattrs) 
    {
      drop.levels <- function(dataframe) {
        dataframe[] <- lapply(dataframe, function(x) x[, drop = TRUE])
        return(dataframe)
      }
      runattrs <- drop.levels(reshape(runattrs, direction = "wide", 
                                      idvar = "runid", timevar = "attrname"))
      names(runattrs) <- sapply(names(runattrs), function(name) sub("^attrvalue\\.", 
                                                                    "", name))
      runattrs
    }
  
  runattrs <- getRunsInWideFormat(dataset$runattrs)
  scalars_ci <- merge(runattrs, df_ci)
  scalars_ci
  rowNames = apply(scalars_ci[rows], 1, paste, collapse = ".")
  columnNames = apply(scalars_ci[columns], 1, paste, collapse = ".")
  #tapply(scalars$value, list(rowNames, columnNames), aggregate)
  m <- tapply(scalars_ci$value, list(rowNames, columnNames), aggregate)
  m
  
  # -- Function content
#     runattrs <- getRunsInWideFormat(dataset$runattrs)
#     scalars_hit <- merge(runattrs, dataset_hit$scalars)
#     
#     scalars_miss <- merge(runattrs, dataset_miss$scalars)
#     
#     scalars_combine <- cbind(scalars_hit, scalars_miss$value)
#   
#     rowNames = apply(scalars[rows], 1, paste, collapse = ".")
#     columnNames = apply(scalars[columns], 1, paste, collapse = ".")
    #tapply(scalars$value, list(rowNames, columnNames), aggregate)
#     m <- tapply(scalars$value, list(rowNames, columnNames), aggregate)
#     print(m)
  
#   d_maxNOP <- makeBarChartDataset(dataset_maxNOP, rows=c('measurement'), columns=c('name'))
# 
# d_Hit <- makeBarChartDataset(dataset_Hit, rows=c('measurement'), columns=c('name'))
# print(d_Hit)
# 
# d_Miss <- makeBarChartDataset(dataset_Miss, rows=c('measurement'), columns=c('name'))
#print(d_Miss)

# CI <- d_Hit / (d_Hit + d_Miss)
# print(CI)
# 
# m_ci <- cbind(d_maxNOP, CI)
# print(m_ci)

#plotBarChart(CI, Legend.Display='true', Legend.Anchoring='NorthWest')

