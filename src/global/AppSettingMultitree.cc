#include "AppSettingMultitree.h"

Define_Module(AppSettingMultitree)

AppSettingMultitree::AppSettingMultitree(){}
AppSettingMultitree::~AppSettingMultitree(){}
void AppSettingMultitree::finish(){}

void AppSettingMultitree::handleMessage(cMessage* msg)
{
    throw cException("AppSettingMultitree does not process messages!");
}

void AppSettingMultitree::initialize()
{
	param_numStripes				= par("numStripes");
    param_bufferMapSize_seconds		= par("bufferMapSize").longValue();
	param_chunkSize					= par("chunkSize").longValue();
	param_videoStreamBitRate		= par("videoStreamBitRate").longValue();

    param_intervalNewChunk = ((double)param_chunkSize * 8.0) / param_videoStreamBitRate;

    m_videoStreamChunkRate = param_videoStreamBitRate / param_chunkSize / 8;
    param_bufferMapSizeChunk = param_bufferMapSize_seconds * m_videoStreamChunkRate;

	WATCH(param_numStripes);
	WATCH(param_chunkSize);
	WATCH(param_videoStreamBitRate);
	WATCH(param_intervalNewChunk);
	WATCH(m_videoStreamChunkRate);
}
