#include "WorldlinkMgr.h"

WorldlinkMgr::WorldlinkMgr() {
}

WorldlinkMgr::~WorldlinkMgr() {
	clearWorlds();
}

void WorldlinkMgr::clearWorlds() {
	std::map<WORLD_ID, s_link*>::iterator p;
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
			WORLD_ID id = database::toInt(row[0]);
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
}

void WorldlinkMgr::createLink(WORLD_ID id) {
	m_data[id]->online = true;
}

void WorldlinkMgr::destroyLink(WORLD_ID id) {
	m_data[id]->online = false;
}

bool WorldlinkMgr::isOnline(WORLD_ID id) {
	return m_data[id]->online;
}

void WorldlinkMgr::reloadWorldsFromDB(database::connection db) {
	clearWorlds();
	loadWorldsFromDB(db);
}
