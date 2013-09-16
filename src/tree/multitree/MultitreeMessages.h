//
// =============================================================================
// OSSIM : A Generic Simulation Framework for Overlay Streaming
// =============================================================================
//
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Project Info: http://www.p2p.tu-darmstadt.de/research/ossim
//
// OSSIM is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// OSSIM is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <http://www.gnu.org/licenses/>.

// -----------------------------------------------------------------------------
// MultitreeMessages.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Julian Wulfheide;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

#ifndef MULTITREEMESSAGE
#define MULTITREEMESSAGE true

struct ConnectRequest
{
	int stripe;
	std::vector<int> numSuccessors;
	long lastReceivedChunk;
	IPvXAddress currentParent;
	std::vector<IPvXAddress> lastRequests;
};

struct ConnectConfirm
{
	int stripe;
	IPvXAddress alternativeParent;	
};

struct DisconnectRequest
{
	int stripe;
	IPvXAddress alternativeParent;
};

struct PassNodeRequest
{
	int stripe;
	int remainingBW;
	float threshold;
	float dependencyFactor;
};

struct SuccessorInfo
{
	int stripe;
	int numSuccessors;
};

#endif
