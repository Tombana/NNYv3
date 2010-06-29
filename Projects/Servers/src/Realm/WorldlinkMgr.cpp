#include "WorldlinkMgr.h"

WorldlinkMgr::WorldlinkMgr() {
}

WorldlinkMgr::~WorldlinkMgr() {
	clearWorlds();
}

void WorldlinkMgr::clearWorlds() {
	std::map<DWORD, s_link*>::iterator p;
	for(p=m_data.begin(); p != m_data.end(); p++) {
		delete p->second;
	}
	m_data.clear();
}

void WorldlinkMgr::loadWorldsFromDB(database::connection db) {
	database::result result = database::query(db, "SELECT id,ipv4,port,name,link_key FROM servers", database::STORE_RESULT);
	if (result) {
		database::row row;
		while (row = database::fetch_row(result)) {
			//We do the conversion to integer only once and save it
			DWORD id = database::toInt(row[0]);
			//We create a new link structure
			m_data[id] = new s_link;
			//We fill the structure
			m_data[id]->ipv4		= row[1];
			m_data[id]->port		= database::toInt(row[2]);
			m_data[id]->name		= row[3];
			m_data[id]->link_key	= row[4];
			m_data[id]->online		= false;
		}
		database::free_result(result);
	}
	preparePacket(); //generate packet for later use
}

void WorldlinkMgr::preparePacket() {
	m_generatedPacket.clear();
	std::map<DWORD, s_link*>::iterator p;
	for(p=m_data.begin(); p != m_data.end(); p++) {
		m_generatedPacket.add<CMD>(PCKT_R_WORLD); //CMD
		m_generatedPacket.addString(p->second->ipv4); //ipv4
		m_generatedPacket.add<PORT>(p->second->port); //port
		m_generatedPacket.addString(p->second->name); //name
		m_generatedPacket.addBool(p->second->online); //online
	}
	m_generatedPacket.add<CMD>(PCKT_R_WORLD_EOF); //EOF
}

Packet& WorldlinkMgr::getGeneratedPacket() {
	return m_generatedPacket;
}

void WorldlinkMgr::createLink(DWORD id) {
	m_data[id]->online = true;
	preparePacket(); //regenerate packet for later use
}

void WorldlinkMgr::destroyLink(DWORD id) {
	m_data[id]->online = false;
	preparePacket(); //regenerate packet for later use
}

bool WorldlinkMgr::isOnline(DWORD id) {
	return m_data[id]->online;
}

void WorldlinkMgr::reloadWorldsFromDB(database::connection db) {
	clearWorlds();
	loadWorldsFromDB(db);
	preparePacket(); //regenerate packet for later use
}
