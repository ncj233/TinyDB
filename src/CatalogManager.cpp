#include "CatalogManager.h"

using namespace std;


CatalogManager::CatalogManager()
{
	fstream file;
	file.open("database", ios::binary | ios::in);
	if (!file.is_open())
		return;
	int index_num = file.get();
	int table_num = file.get();
	for (int i = 0; i < index_num; i++)
		index.push_back(IndexName(file));
	for (int i = 0; i < table_num; i++)
		table.push_back(Table(file));
	file.close();
}

void CatalogManager::save()
{
	fstream file;
	file.open("datebase", ios::binary | ios::out);
	assert(file.is_open());
}

