#ifndef _H_EVENTREADERBASE_H
#define _H_EVENTREADERBASE_H

#include "NtupleInfo.h"



class RootFileReaderBase  {
	long d_startEvent;
	long d_endEvent;
	long d_iEntry;
	long d_iEvent;
	long d_lastID;
	long d_startTrueEvent;
	long d_maxTrueEvent;
	NtupleInfo<MAX_NBR_PARTICLES>* d_storage;
 protected:
	TChain* d_fin ;
	bool d_initialised;
public:
	//! Constructor
	RootFileReaderBase();
	//! Constructor
	RootFileReaderBase(NtupleInfo<MAX_NBR_PARTICLES>& NI,const std::string& treeName="t3",bool doublePrecision=false,bool withNcount=false);
	/** Reads the next entry and returns true in case of success, false otherwise
	(including reaching the end of the file).*/
	bool readNextEntry(NtupleInfo<MAX_NBR_PARTICLES>& NI);
	/** Sets the index of the entry at which reading will start.
	\param index 1-based index at which the reading will start (the \\varref{index}-th entry will be read at the next call of {\\tt nextEntry()})
	 * */
	void setStartEntryIndex(long index){d_startEvent=index-1;d_iEntry = d_startEvent;d_startTrueEvent=0;resetEventCounter();};
	/** Sets the index of the entry at which reading will stop.
	\param index 1-based index at which the reading will stop (the \\varref{index}-th entry will not be read)
	 * */
	void setEndEntryIndex(long index){d_endEvent=index-1;d_maxTrueEvent=0;};
	/** Sets the number for the event sequence counter at which the library will start reading in events
	 (phase-space configuration\--counter-configuration
	groups of entries, or simply entries for contribution types with one entry per event).
	 \param count is a 1-based sequence number specifying the first event that will be read.
	 */
	void setStartEvent(long count){d_startTrueEvent=count;d_iEntry = 0 /*I have to start at the beginning to count the events*/;resetEventCounter();};
	/** Sets the reader to stop reading entries when the given number
	 * of events (phase-space configuration\--counter-configuration
	 * groups of entries, or simply entries for contribution types
	 * with one entry per event) have been read.
	 * \param count is a 1-based sequence number specifying the first event that will not be read.
	 * */
	void setMaxEvent(long count);
	/** Returns the (1-based) index of the entry at which reading will start.
	 * */
	long getStartEntryIndex(){return d_startEvent +1;	};
	/** Returns the (1-based) index of the entry at which reading will stop.
	 * */
	long getEndEntryIndex(){return d_endEvent+1;d_maxTrueEvent=0;};
	/** Returns the ID of the event (phase-space configuration\--counter-configuration
	 * group of entries, or simply entries for contribution types with one entry per
	 * event) after which reading will stop.
	 * */
	long getMaxEvent(){return d_maxTrueEvent;};
	/** Returns the total number of entries.
	 * */
	long getNumberOfEntries();
	/** Reads the entry corresponding to the index specified; {\\tt nextEntry\/}() will start from that position.
	 \param index is the index of the entry to be read.
	 * */
	void getEntry(long index);
	/** Returns the index of the next entry.
	 * */
	bool getEntry(long index,NtupleInfo<MAX_NBR_PARTICLES>& NI);
	/** Returns the index of the next entry.
	 * */
	long getIndexOfNextEntry(){return d_iEntry+1;};
	/** Adds a file to the reader.
	 * \param fileName is the name of the file.
	 * */
	virtual void addFile(NtupleInfo<MAX_NBR_PARTICLES>& NI,const std::string& fileName);
	/** Adds a list of files to the reader
	 * \param fileNames is a vector of {\\tt std::string\/} containing the names
	 * of the files to be added, which will be read in the order given.
	 * */
	void addFiles(NtupleInfo<MAX_NBR_PARTICLES>& NI,std::vector<std::string> fileNames);

	//! initialises the reader
	void init(NtupleInfo<MAX_NBR_PARTICLES>& NI,const std::string& treeName="t3",bool doublePrecision=false,bool withNcount=false);

	virtual ~RootFileReaderBase();

private:
	void resetEventCounter(){d_iEvent=0;d_lastID=-1;};
	void initStorage(NtupleInfo<MAX_NBR_PARTICLES>& NI,const std::string& fileName );
};


#endif /* _H_EVENTREADERBASE_H */
