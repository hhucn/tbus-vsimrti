/**
 * @author Tobias Krauthoff <tobias.krauthoff@hhu.de>
 * @date 22.12.2013
 * Copyright (c) Heinrich-Heine-Universität Düsseldorf. All rights reserved.
 */

// some Values
#define kZERO									0.0
#define kSECOND									1.0
#define kEPS3									10e-3 //bei wieviel Bytes ignoriert Norbert Rundungsfehler?
#define kEPS7									10e-7
#define kDEFAULTDELAY							10e-3
#define kMULTIPLIER_S_TO_NS						1.0E9
#define kMULTIPLIER_S_TO_MS						1.0E3
#define kDIVIDER_KB_TO_B						1.0E3

// some string
#define kSTRING_UPLOAD							"Upload"
#define kSTRING_DOWNLOAD						"Download"
#define kSTRING_CONTROL_CRSQ					"ControlCRSQ"
#define kSTRING_CONTROL_CDSQ					"ControlCDSQ"
#define kSTRING_CONTROL_CRRQ					"ControlCRRQ"
#define kSTRING_CONTROL_CDRQ					"ControlCDRQ"
#define kSTRING_MOBILENODE						"MN"
#define kSTRING_INTERNETNODE					"IN"
#define kSTRING_ROUTER							"Router"
#define kSTRING_SEND							"send at"
#define kSTRING_LOG								"logged at"
#define kSTRING_DROP							"dropped at"
#define kSTRING_DROPBO							"drop BO"
#define kSTRING_LOST							"lost at"
#define kSTRING_RECEIVED						"rec at"
#define kSTRING_ROUTED							"routed"
#define kSTRING_NEWDATARATEPOINTER				"newDataratePointer"
#define kSTRING_NEWBBDELAYPOINTER				"newBbdelayPointer"
#define kSTRING_STARTTRAIN						"startTrain"
#define kSTRING_PPS								"pulsePerSecond"
#define kSTRING_SYN								"syn"
#define kSTRING_UPDATEVECTORS					"updateVectors"
#define kSTRING_CDSQ							"CDSQ"
#define kSTRING_CRSQ							"CRSQ"
#define kSTRING_CDRQ							"CDRQ"
#define kSTRING_CRRQ							"CRRQ"
#define kSTRING_CDSQ_SENDHEAD					"CDSQ send head"
#define kSTRING_CRSQ_SENDHEAD					"CRSQ send head"
#define kSTRING_CDRQ_SENDHEAD					"CDRQ send head"
#define kSTRING_CRRQ_SENDHEAD					"CRRQ send head"

//strings for filennames
#define kSTRING_FILE_CRSQ_SEND					"CRSQ.send"
#define kSTRING_FILE_CDSQ_SEND					"CDSQ.end"
#define kSTRING_FILE_CRRQ_SEND					"CRRQ.send"
#define kSTRING_FILE_CDRQ_SEND					"CDRQ.send"
#define kSTRING_FILE_CRSQ_SENDHEAD				"CRSQ.sendhead"
#define kSTRING_FILE_CDSQ_SENDHEAD				"CDSQ.sendhead"
#define kSTRING_FILE_CRRQ_SENDHEAD				"CRRQ.sendhead"
#define kSTRING_FILE_CDRQ_SENDHEAD				"CDRQ.sendhead"
#define kSTRING_FILE_CRSQ_RECEIVED				"CRSQ.received"
#define kSTRING_FILE_CDSQ_RECEIVED				"CDSQ.received"
#define kSTRING_FILE_CRRQ_RECEIVED				"CRRQ.received"
#define kSTRING_FILE_CDRQ_RECEIVED				"CDRQ.received"
#define kSTRING_FILE_UPLOADDATARATE				"uploaddatarate"
#define kSTRING_FILE_UPLOADBBDELAY				"uploadbbdelay"
#define kSTRING_FILE_UPLOADPACKETS				"uploadpackets"
#define kSTRING_FILE_UPLOADRECEIVED				"uploadrec"
#define kSTRING_FILE_UPLOADSEND					"uploadsend"
#define kSTRING_FILE_UPLOADPACKETTIMESTAMPS     "uploadpackettimestamps"
#define kSTRING_FILE_UPLOADPACKETSBBDELAY       "uploadpacketsbbdelay"
#define kSTRING_FILE_UPLOADPACKETSDATARATE      "uploadpacketsdatarate"
#define kSTRING_FILE_UPLOADCHAR					"uploadchar"
#define kSTRING_FILE_UPLOADSTATS				"uploadstats"
#define kSTRING_FILE_UPLOADSTATS_FIRSTDIFF		"uploadstats.firstdiff"
#define kSTRING_FILE_UPLOADSTATS_LASTDIFF		"uploadstats.lastdiff"
#define kSTRING_FILE_UPLOAD_SIMCDF				"uploadDelaySimCDF"
#define kSTRING_FILE_UPLOAD_REALCDF				"uploadDelayRealCDF"
#define kSTRING_FILE_DOWNLOADDATARATE			"downloaddatarate"
#define kSTRING_FILE_DOWNLOADBBDELAY			"downloadbbdelay"
#define kSTRING_FILE_DOWNLOADPACKETS			"downloadpackets"
#define kSTRING_FILE_DOWNLOADRECEIVED			"downloadrec"
#define kSTRING_FILE_DOWNLOADSEND				"downloadsend"
#define kSTRING_FILE_DOWNLOADPACKETTIMESTAMPS	"downloadpackettimestamps"
#define kSTRING_FILE_DOWNLOADPACKETSBBDELAY		"downloadpacketsbbdelay"
#define kSTRING_FILE_DOWNLOADPACKETSDATARATE	"downloadpacketsdatarate"
#define kSTRING_FILE_DOWNLOADCHAR				"downloadchar"
#define kSTRING_FILE_DOWNLOADSTATS				"downloadstats"
#define kSTRING_FILE_DOWNLOADSTATS_FIRSTDIFF	"downloadstats.firstdiff"
#define kSTRING_FILE_DOWNLOADSTATS_LASTDIFF		"downloadstats.lastdiff"
#define kSTRING_FILE_DOWNLOAD_SIMCDF			"downloadDelaySimCDF"
#define kSTRING_FILE_DOWNLOAD_REALCDF			"downloadDelayRealCDF"
#define kSTRING_FILE_PACKETS					"packets"
#define kSTRING_FILE_PACKETSUPLOAD				"packets.upload"
#define kSTRING_FILE_PACKETSDOWNLOAD			"packets.download"
#define kSTRING_FILE_PACKETSSTATS				"packets.stats"


//ports
#define kGATEOUT_CONTROL_TO_CRSQ				"controlPortCRSQ$o"
#define kGATEOUT_CONTROL_TO_CDSQ				"controlPortCDSQ$o"
#define kGATEOUT_CONTROL_TO_CDRQ				"controlPortCDRQ$o"
#define kGATEOUT_CONTROL_TO_CRRQ				"controlPortCRRQ$o"
#define kGATEIN_CONTROL_FROM_CRSQ				"controlPortCRSQ$i"
#define kGATEIN_CONTROL_FROM_CDSQ				"controlPortCDSQ$i"
#define kGATEIN_CONTROL_FROM_CDRQ				"controlPortCDRQ$i"
#define kGATEIN_CONTROL_FROM_CRRQ				"controlPortCRRQ$i"
#define kGATEIN_CONTROL_ALL_Queues				"controlPort$i"
#define kGATEOUT_CONTROL_ALL_Queues				"controlPort$o"


// Strings for par(..)-attributes
#define kPAR_MAXTRAINLENGTH						"maximalTrainLength"
#define kPAR_IP									"ip"
#define kPAR_PORT								"port"
#define kPAR_SENDINITMSG						"sendInitialMessage"
#define kPAR_SENDLOGMSG							"sendLogFileMessages"
#define kPAR_SIMTIMELIMIT						"simTimeLimit"
#define kPAR_TRAINSENDINTERVALL					"trainIntervall"
#define kPAR_DESITNATIONIP						"destip"
#define kPAR_CHANGEPACKETSDELAY					"changePacketsDelayForTesting"
#define kPAR_WRITINGLOGFILENODE					"writingLogFileNode"
#define kPAR_WRITINGLOGFILEQUEUE				"writingLogFileQueue"
#define kPAR_LOGFILEPREFIX						"outputPathPrefix"
#define kPAR_INPUTPREFIX						"inputPathPrefix"

// Strings for Signals
// Mobile Node
#define kSIGNAL_MOBILE_NODE_TRAVELDELAY			"mobile_node_traveldelay"
#define kSIGNAL_MOBILE_NODE_SENDINGTIME			"mobile_node_sendingtime"
#define kSIGNAL_MOBILE_NODE_ARRIVALTIME			"mobile_node_arrivaltime"
#define kSIGNAL_MOBILE_NODE_TRAINLENGTH			"mobile_node_trainlength"
#define kSIGNAL_MOBILE_NODE_DROPMEASURED		"mobile_node_drop_measured"
#define kSIGNAL_MOBILE_NODE_DROPSIMULATED		"mobile_node_drop_simulated"

// Internet Node
#define kSIGNAL_INET_NODE_TRAVELDELAY			"inet_node_traveldelay"
#define kSIGNAL_INET_NODE_DROPMEASURED			"inet_node_drop_measured"
#define kSIGNAL_INET_NODE_DROPSIMULATED			"inet_node_drop_simulated"

// C(lient data)R(ate)S(end)Q(ueue)
#define kSIGNAL_CRSQ_DATARATE					"CRSQ_datarate"
#define kSIGNAL_CRSQ_LOSSPROB					"CRSQ_lossprob"
#define kSIGNAL_CRSQ_LOSSPROB_RND				"CRSQ_lossprob_rnd"
#define kSIGNAL_CRSQ_PACKET_DATARATE_DELAY		"CRSQ_packet_datarate_delay"
#define kSIGNAL_CRSQ_DROPMEASURED				"CRSQ_drop_measured"
#define kSIGNAL_CRSQ_DROPSIMULATED				"CRSQ_drop_simulated"

// C(lient)D(elay)S(end)Q(ueue)
#define kSIGNAL_CDSQ_BBDELAY					"CDSQ_bbdelay"

// C(lient)D(elay)R(eceive)Q(ueue)
#define kSIGNAL_CDRQ_BBDELAY					"CDRQ_bbdelay"

// C(lient data)R(ate)R(eceive)Q(ueue)
#define kSIGNAL_CRRQ_DATARATE					"CRRQ_datarate"
#define kSIGNAL_CRRQ_LOSSPROB					"CRRQ_lossprob"
#define kSIGNAL_CRRQ_LOSSPROB_RND				"CRRQ_lossprob_rnd"
#define kSIGNAL_CRRQ_PACKET_DATARATE_DELAY		"CRRQ_packet_datarate_delay"
#define kSIGNAL_CRRQ_DROPMEASURED				"CRRQ_drop_measured"
#define kSIGNAL_CRRQ_DROPSIMULATED				"CRRQ_drop_simulated"

// Segments
#define kDSL_PACKETSIZE							1486
#define kUMTS_PACKETSIZE						1500
