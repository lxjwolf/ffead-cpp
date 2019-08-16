/*
 * CommonUtils.cpp
 *
 *  Created on: 10-Dec-2014
 *      Author: sumeetc
 */

#include "CommonUtils.h"

CommonUtils* CommonUtils::instance = NULL;
std::string_view CommonUtils::BLANK = {""};
std::atomic<long long> CommonUtils::tsPoll = 0;
std::atomic<long long> CommonUtils::tsPoll1 = 0;
std::atomic<long long> CommonUtils::tsProcess = 0;
std::atomic<long long> CommonUtils::tsRead = 0;
std::atomic<long long> CommonUtils::tsService = 0;
std::atomic<long long> CommonUtils::tsWrite = 0;
std::atomic<long long> CommonUtils::tsServicePre = 0;
std::atomic<long long> CommonUtils::tsServiceCors = 0;
std::atomic<long long> CommonUtils::tsServiceSec = 0;
std::atomic<long long> CommonUtils::tsServiceFlt = 0;
std::atomic<long long> CommonUtils::tsServiceCnt = 0;
std::atomic<long long> CommonUtils::tsServiceExt = 0;
std::atomic<long long> CommonUtils::tsServicePost = 0;
std::atomic<long long> CommonUtils::tsContMpg = 0;
std::atomic<long long> CommonUtils::tsContPath = 0;
std::atomic<long long> CommonUtils::tsContExt = 0;
std::atomic<long long> CommonUtils::tsContExec = 0;
std::atomic<long long> CommonUtils::tsContRstLkp = 0;
std::atomic<long long> CommonUtils::tsContRstCsiLkp = 0;
std::atomic<long long> CommonUtils::tsContRstInsLkp = 0;
std::atomic<long long> CommonUtils::tsContRstPrsArgs = 0;
std::atomic<long long> CommonUtils::tsContRstExec = 0;
std::atomic<long long> CommonUtils::tsContRstSer = 0;
std::atomic<long long> CommonUtils::cSocks = 0;
std::atomic<long long> CommonUtils::cReqs = 0;
std::atomic<long long> CommonUtils::cResps = 0;

CommonUtils::CommonUtils() {
}

CommonUtils::~CommonUtils() {
}

CommonUtils* CommonUtils::getInstance() {
	if(instance==NULL) {
		instance = new CommonUtils;
	}
	return instance;
}

void CommonUtils::clearInstance() {
	if(instance!=NULL) {
		delete instance;
	}
}

int CommonUtils::getProcessorCount() {
#if defined(OS_MINGW)
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#elif defined(OS_DARWIN)
    int nm[2];
    size_t len = 4;
    uint32_t count;

    nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);

    if(count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if(count < 1) { count = 1; }
    }
    return count;
#elif defined(OS_HPUX)
    return  mpctl(MPC_GETNUMSPUS, NULL, NULL);
#elif defined(OS_IRIX)
    return sysconf(_SC_NPROC_ONLN);
#else
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

void CommonUtils::addContext(std::string appName) {
	getInstance()->appNames.push_back(appName);
}

void CommonUtils::setAppName(const std::string& appName)
{
	for(int i=0;i<(int)getInstance()->appNames.size();i++) {
		if(appName==getInstance()->appNames.at(i)) {
			getInstance()->contextName.reset(&getInstance()->appNames.at(i));
			return;
		}
	}
}

void CommonUtils::loadMimeTypes(const std::string& file)
{
	if(getInstance()->mimeTypes.size()>0)return;
	PropFileReader pread;
	propMap m = pread.getProperties(file);
	propMap::iterator i;
	for(i=m.begin();i!=m.end();++i) {
		getInstance()->mimeTypes[i->first] = i->second;
	}
}

std::string_view CommonUtils::getMimeType(std::string_view extension)
{
	if(getInstance()->mimeTypes.find(extension)!=getInstance()->mimeTypes.end())
	{
		return getInstance()->mimeTypes[extension];
	}
	return BLANK;
}

void CommonUtils::loadLocales(const std::string& file)
{
	if(getInstance()->locales.size()>0)return;
	PropFileReader pread;
	propMap m = pread.getProperties(file);
	propMap::iterator i;
	for(i=m.begin();i!=m.end();++i) {
		getInstance()->locales[i->first] = i->second;
	}
}

std::string_view CommonUtils::getLocale(std::string_view abbrev)
{
	if(getInstance()->locales.find(abbrev)!=getInstance()->locales.end())
	{
		return getInstance()->locales[abbrev];
	}
	return BLANK;
}

std::string CommonUtils::getAppName(const std::string& appName)
{
	std::string appn = appName;
	void* tlcn = getInstance()->contextName.get();
	if(appn=="" && tlcn!=NULL)
	{
		appn = *(std::string*)tlcn;
	}
	StringUtil::replaceAll(appn, "-", "_");
	RegexUtil::replace(appn, "[^a-zA-Z0-9_]+", "");
	return appn;
}

unsigned long long CommonUtils::charArrayToULongLong(const std::string& l)
{
	unsigned long long t = 0;
	int ind = l.length();
	for (int i = 0; i < ind; i++)
	{
		t = (t << 8) + (l.at(i) & 0xff);
	}
	return t;
}

unsigned long long CommonUtils::charArrayToULongLong(const std::string& l, int ind)
{
	unsigned long long t = 0;
	for (int i = 0; i < ind; i++)
	{
		t = (t << 8) + (l.at(i) & 0xff);
	}
	return t;
}

unsigned long long CommonUtils::charArrayToULongLong(const std::vector<unsigned char>& l)
{
	unsigned long long t = 0;
	int ind = l.size();
	for (int i = 0; i < ind; i++)
	{
		t = (t << 8) + (l.at(i) & 0xff);
	}
	return t;
}

std::string CommonUtils::ulonglongTocharArray(const unsigned long long& lon, const int& provind)
{
	int ind;
	if(lon<256)
	{
		ind = 1;
	}
	else if(lon<65536)
	{
		ind = 2;
	}
	else if(lon<16777216)
	{
		ind = 3;
	}
	else if(lon<4294967296ULL)
	{
		ind = 4;
	}
	else if(lon<1099511627776ULL)
	{
		ind = 5;
	}
	else if(lon<281474976710656ULL)
	{
		ind = 6;
	}
	else if(lon<72057594037927936ULL)
	{
		ind = 7;
	}
	else
	{
		ind = 8;
	}

	if(provind!=-1 && ind<provind) {
		ind = provind;
	}

	std::string result;
	for (int i = 0; i<ind; i++)
	{
		int offset = (ind - 1 - i) * 8;
		result.push_back((char) ((lon >> offset) & 0xFF));
	}
	return result;
}

void CommonUtils::printBinary(const std::string& encv, const bool& isNL)
{
	for (int var = 0; var < (int)encv.size(); ++var) {
		std::bitset<8> bits((unsigned char)encv.at(var));
		std::cout << bits.to_string();
		if(isNL)
		{
			std::cout << std::endl;
		}
	}
}

std::string CommonUtils::toBinary(const std::string& encv)
{
	std::string decv;
	for (int var = 0; var < (int)encv.size(); ++var) {
		std::bitset<8> bits((unsigned char)encv.at(var));
		decv.append(bits.to_string());
	}
	return decv;
}

void CommonUtils::printMap(const std::map<std::string, std::string>& mp)
{
	std::map<std::string, std::string>::const_iterator it;
	for (it=mp.begin();it!=mp.end();++it) {
		std::cout << it->first << " = " << it->second << std::endl;
	}
}

void CommonUtils::printMap(const std::map<int, std::map<std::string, std::string> >& mp)
{
	std::map<int, std::map<std::string, std::string> >::const_iterator it;
	for (it=mp.begin();it!=mp.end();++it) {
		std::cout << it->first << " = " << it->second.begin()->first << ":" << it->second.begin()->second << std::endl;
	}
}

void CommonUtils::printMap(const std::map<std::string, int>& mp)
{
	std::map<std::string, int>::const_iterator it;
	for (it=mp.begin();it!=mp.end();++it) {
		std::cout << it->first << " = " << it->second << std::endl;
	}
}

void CommonUtils::printHEX(const std::string& encv)
{
	for (int var = 0; var < (int)encv.size(); ++var) {
		char hexVal[20];
		memset(hexVal, 0, sizeof(hexVal));
		sprintf(hexVal, "%x", (unsigned char)encv.at(var));
		std::string hexstr(hexVal);
		if(hexstr.length()==1)
			hexstr = "0" + hexstr;
		std::cout << hexstr;
		if(var%2==1)
			std::cout << " ";
	}
	std::cout << std::endl;
}

std::string CommonUtils::toHEX(const uint32_t& num)
{
	char hexVal[20];
	memset(hexVal, 0, sizeof(hexVal));
	sprintf(hexVal, "%x", num);
	std::string hexstr("0x");
	hexstr.append(hexVal);
	if(hexstr.length()==1)
		hexstr = "0" + hexstr;
	hexstr += "u";
	return hexstr;
}

std::string CommonUtils::xorEncryptDecrypt(const std::string& toEncrypt, const uint32_t& maskingKey) {
	std::string output = toEncrypt;
	std::string maskKeystr = ulonglongTocharArray(maskingKey, 4);

    for (int i = 0; i < (int)toEncrypt.size(); i++)
    {
    	output[i] = toEncrypt[i] ^ maskKeystr[i%4];
    }
    return output;
}

void CommonUtils::listFiles(std::vector<std::string>& files, const std::string& cwd, const std::string& suffix, const bool& isAbs)
{
	files.clear();
	struct dirent *entry;
	DIR *dir;
	dir = opendir(cwd.c_str());
	if(dir==NULL)return;

	while ((entry = readdir (dir)) != NULL) {
		std::string file = isAbs?(cwd+"/"):"";
		file.append(entry->d_name);
		std::string f = cwd + "/" + std::string(entry->d_name);
		struct stat sb;
		stat (f.c_str(), &sb);
		if(S_ISREG(sb.st_mode) && StringUtil::endsWith(file, suffix)) {
			RegexUtil::replace(file,"[/]+","/");
			files.push_back(file);
		} else if(S_ISDIR(sb.st_mode) && suffix=="/") {
			if(isAbs && !StringUtil::endsWith(file, "/.") && !StringUtil::endsWith(file, "/..")) {
				file += "/";
				RegexUtil::replace(file,"[/]+","/");
				files.push_back(file);
			} else if(!isAbs && file!="." && file!="..") {
				files.push_back(file);
			}
		}
	}
	closedir(dir);
}

std::vector<std::string> CommonUtils::getFiles(const std::string& cwd, const std::string& suffix, const bool& isAbs)
{
	std::vector<std::string> files;
	listFiles(files, cwd, suffix, isAbs);
	return files;
}

void CommonUtils::printStats() {
	Logger logger = LoggerFactory::getLogger("CommonUtils");
	std::string a = ("Connections (Sockets: "+CastUtil::lexical_cast<std::string>(cSocks) + ", Open Sockets: " + CastUtil::lexical_cast<std::string>(SocketInterface::openSocks)
			+", Requests: "+CastUtil::lexical_cast<std::string>(cReqs)+", Responses: "+CastUtil::lexical_cast<std::string>(cResps)+")\n");
	logger.info(a);
	std::string b = ("E-E Total (EL_Pre: "+CastUtil::lexical_cast<std::string>(tsPoll1)+", EL_Wait: "+CastUtil::lexical_cast<std::string>(tsPoll)+
			", EL_Process: "+CastUtil::lexical_cast<std::string>(tsProcess)+", Read: "+CastUtil::lexical_cast<std::string>(tsRead)+
			", Write: "+CastUtil::lexical_cast<std::string>(tsWrite)+", Service: "+CastUtil::lexical_cast<std::string>(tsService)+")\n");
	logger.info(b);
	if(cReqs>0) {
		std::string c = ("E-E Average (EL_Pre: "+CastUtil::lexical_cast<std::string>(tsPoll1/cSocks)+", EL_Wait: "+CastUtil::lexical_cast<std::string>(tsPoll/cSocks)+
				", EL_Process: "+ CastUtil::lexical_cast<std::string>(tsProcess/cSocks)+", Read: "+CastUtil::lexical_cast<std::string>(tsRead/cReqs)+
				", Write: "+CastUtil::lexical_cast<std::string>(tsWrite/cReqs)+", Service: "+ CastUtil::lexical_cast<std::string>(tsService/cReqs)+")\n");
		logger.info(c);
	}
	std::string d = ("Service Total (Pre: "+CastUtil::lexical_cast<std::string>(tsServicePre)+", Cors: "+CastUtil::lexical_cast<std::string>(tsServiceCors)+
			", Security: "+CastUtil::lexical_cast<std::string>(tsServiceSec) + ", Filter: "+CastUtil::lexical_cast<std::string>(tsServiceFlt)+
			", Controller: "+CastUtil::lexical_cast<std::string>(tsServiceCnt)+", Ext: "+CastUtil::lexical_cast<std::string>(tsServiceExt)+
			", Post: "+CastUtil::lexical_cast<std::string>(tsServicePost)+")\n");
	logger.info(d);
	if(cReqs>0) {
		std::string e = ("Service Average (Pre: "+CastUtil::lexical_cast<std::string>(tsServicePre/cReqs)+", Cors: "+CastUtil::lexical_cast<std::string>(tsServiceCors/cReqs)+
				", Security: "+ CastUtil::lexical_cast<std::string>(tsServiceSec/cReqs) + ", Filter: "+CastUtil::lexical_cast<std::string>(tsServiceFlt/cReqs)+
				", Controller: "+CastUtil::lexical_cast<std::string>(tsServiceCnt/cReqs)+ ", Ext: "+CastUtil::lexical_cast<std::string>(tsServiceExt/cReqs)+
				", Post: "+CastUtil::lexical_cast<std::string>(tsServicePost/cReqs)+")\n");
		logger.info(e);
	}
	std::string f = ("Controller Total (Cont_Cond: "+CastUtil::lexical_cast<std::string>(tsContMpg)+", Mapg_Cond: "+CastUtil::lexical_cast<std::string>(tsContPath)+
			", Ext_Cond: "+CastUtil::lexical_cast<std::string>(tsContExt) + ", Cont_Exec: "+CastUtil::lexical_cast<std::string>(tsContExec)+
			", Rest_Lkp: "+CastUtil::lexical_cast<std::string>(tsContRstLkp)+", Rest_CsiLkp: "+CastUtil::lexical_cast<std::string>(tsContRstCsiLkp)+
			", Rest_InsLkp: "+CastUtil::lexical_cast<std::string>(tsContRstInsLkp)+", Rest_PrsArgs: "+CastUtil::lexical_cast<std::string>(tsContRstPrsArgs)+
			", Rest_Exec: "+CastUtil::lexical_cast<std::string>(tsContRstExec)+ ", Rest_Ser: "+CastUtil::lexical_cast<std::string>(tsContRstSer)+")\n");
	logger.info(f);
	if(cReqs>0) {
		std::string g = ("Controller Average (Cont_Cond: "+CastUtil::lexical_cast<std::string>(tsContMpg/cReqs)+
				", Mapg_Cond: "+CastUtil::lexical_cast<std::string>(tsContPath/cReqs)+", Ext_Cond: "+CastUtil::lexical_cast<std::string>(tsContExt/cReqs) +
				", Cont_Exec: "+CastUtil::lexical_cast<std::string>(tsContExec/cReqs)+", Rest_Lkp: "+CastUtil::lexical_cast<std::string>(tsContRstLkp/cReqs)+
				", Rest_CsiLkp: "+CastUtil::lexical_cast<std::string>(tsContRstCsiLkp/cReqs)+ ", Rest_InsLkp: "+CastUtil::lexical_cast<std::string>(tsContRstInsLkp/cReqs)+
				", Rest_PrsArgs: "+CastUtil::lexical_cast<std::string>(tsContRstPrsArgs/cReqs)+", Rest_Exec: "+CastUtil::lexical_cast<std::string>(tsContRstExec/cReqs)+
				", Rest_Ser: "+CastUtil::lexical_cast<std::string>(tsContRstSer/cReqs)+")\n");
		logger.info(g);
	}
}
