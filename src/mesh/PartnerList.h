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
// PartnerList.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#ifndef PARTNERLIST_H_
#define PARTNERLIST_H_

#include "IPvXAddress.h"
#include "NeighborInfo.h"

class PartnerList : public cSimpleModule {
//class PartnerList {
public:
    PartnerList();
    virtual ~PartnerList();

protected:
  virtual int numInitStages() const  {return 4;}
  virtual void initialize(int stage);
  virtual void finish();

  // Raises an error.
  virtual void handleMessage(cMessage *);

public:
    /**
     * For debugging
     */
    void print() const;
    void printAllSendBm() const;

    /** @name Neighbor List manipulation */
    //@{

    /**
     * Checks if a peer, given its address, is a neighbor.
     */
    virtual bool isPartner(const IPvXAddress& dest) const; //FIXME: could be obsolete
    bool have(const IPvXAddress& addr) const;

    /**
     * Returns the total number of Neighbors in the list
     */
    virtual int getSize() const;

    /**
     * Adds a Neighbor's address to the table. Note that once added, addresses
     * cannot be modified; you must delete and re-add them instead.
     */
    virtual void addAddress(const IPvXAddress &address);

    void addAddress(const IPvXAddress &addr, double upBw, int nChunk);

    /**
     * Adds a Neighbor's address to the table. Note that once added, addresses
     * cannot be modified; you must delete and re-add them instead.
     * This version is an addition to the previous one, with extra information about peer's upload bandwidth
     */
    virtual void addAddress(const IPvXAddress &addr, double upBw);

    /**
     * Deletes the given Neighbor's address from the table.
     * Returns true if the address was deleted correctly, false if it was
     * not in the table.
     */
    virtual bool deleteAddress(const IPvXAddress &address);

    /**
     * Utility function: Returns a vector of /N/ addresses of the active nodes.
     */
    virtual std::vector<IPvXAddress> getAddressList() const;

    //virtual void updateNeighborInfo(NeighborInfo *);

   bool hasAddress(const IPvXAddress & address) const;
   NeighborInfo * getNeighborInfo(const IPvXAddress &address) const;

    /*
     *
     */
    virtual int updateStoredBufferMap(IPvXAddress, BufferMap *);

//    virtual void updateKeepAliveTimestamp(IPvXAddress addr, long timestamp);

    void getHolderList(SEQUENCE_NUMBER_T seq_num, std::vector<IPvXAddress> &holderList);
    int getNumberOfHolder(SEQUENCE_NUMBER_T seq_num);

    void clearAllSendBm(void);
    void updateBoundSendBm(SEQUENCE_NUMBER_T head, SEQUENCE_NUMBER_T start, SEQUENCE_NUMBER_T end) const;
    void updateBoundSendBm(SEQUENCE_NUMBER_T start, SEQUENCE_NUMBER_T end) const;
    void resetNChunkScheduled();

    void clearAllTimeBudget(void);

    void resetAllAvailableTime(SEQUENCE_NUMBER_T vb_start, SEQUENCE_NUMBER_T win_start, double chunkInterval);

   void setElementSendBm(IPvXAddress addr, SEQUENCE_NUMBER_T seq_num, bool val);

   double getUpBw(IPvXAddress addr);

   bool updateChunkAvailTime(IPvXAddress addr, SEQUENCE_NUMBER_T seq_num, double time);
   double getChunkAvailTime(IPvXAddress addr, SEQUENCE_NUMBER_T seq_num);

    //@}

    // -- For debugging --
    void printRecvBm(const IPvXAddress &address);
    void printSendBm(const IPvXAddress &address);

private:
    //typedef std::map<IPvXAddress, NeighborInfo *> AddressList;
    typedef std::map<IPvXAddress, NeighborInfo> AddressList;
    mutable AddressList m_map;
    int m_bufferSize;

    friend class DonetPeer;
    friend class DonetBase;
    friend class DonetSource;

};

#endif /* PARTNERLIST_H_ */
