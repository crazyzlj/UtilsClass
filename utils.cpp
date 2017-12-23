#ifndef CLS_UTILS

#include "utils.h"

/************ utilsTime ******************/
double utilsTime::TimeCounting() {
#ifdef windows
    LARGE_INTEGER li;
    if (QueryPerformanceFrequency(&li)) /// CPU supported
    {
        double PCFreq = 0.;
        PCFreq = double(li.QuadPart);
        QueryPerformanceCounter(&li);
        return (double) li.QuadPart / PCFreq; // seconds
    } else {
        return (double) clock() / CLK_TCK;
    }
#else
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return (double) tv.tv_sec + (double) tv.tv_usec / 1000000.;
#endif /* windows */
}

string utilsTime::ConvertToString(const time_t *date) {
    struct tm dateInfo;
#ifdef windows
    localtime_s(&dateInfo, date);
#else
    localtime_r(date, &dateInfo);
#endif /* windows */
    if (dateInfo.tm_isdst > 0) {
        dateInfo.tm_hour -= 1;
    }

    char dateString[11];
    strftime(dateString, 11, "%Y-%m-%d", &dateInfo);

    return string(dateString);
}

string utilsTime::ConvertToString2(const time_t *date) {
    struct tm dateInfo;
#ifdef windows
    localtime_s(&dateInfo, date);
#else
    localtime_r(date, &dateInfo);
#endif /* windows */
    if (dateInfo.tm_isdst > 0) {
        if (dateInfo.tm_hour != 0) {
            dateInfo.tm_hour -= 1;
        } else {
            dateInfo.tm_hour = 23;
            dateInfo.tm_mday -= 1;
            if (dateInfo.tm_mday == 0) {
                dateInfo.tm_mon -= 1;

                if (dateInfo.tm_mon == 0) {
                    dateInfo.tm_year -= 1;
                    dateInfo.tm_mon = 12;
                    dateInfo.tm_mday = 31;
                } else {
                    if (utilsTime::isLeapYear(dateInfo.tm_year)) {
                        dateInfo.tm_mday = daysOfMonth[dateInfo.tm_mon] + 1;
                    } else {
                        dateInfo.tm_mday = daysOfMonth[dateInfo.tm_mon];
                    }
                }

            }
        }
    }
    char dateString[30];
    strftime(dateString, 30, "%Y-%m-%d %X", &dateInfo);

    string s(dateString);
    return s;
}

time_t utilsTime::ConvertToTime(const string &strDate, string const &format, bool includeHour) {
    struct tm *timeinfo;
    time_t t(0);
    if (utilsString::StringMatch(strDate, "")) {
        return t;
    }
    int yr;
    int mn;
    int dy;
    int hr = 0;

    try {
        if (includeHour) {
            stringscanf(strDate.c_str(), format.c_str(), &yr, &mn, &dy, &hr);
        } else {
            stringscanf(strDate.c_str(), format.c_str(), &yr, &mn, &dy);
        }

        timeinfo = new struct tm;
        timeinfo->tm_year = yr - 1900;
        timeinfo->tm_mon = mn - 1;
        timeinfo->tm_mday = dy;
        timeinfo->tm_hour = hr;
        timeinfo->tm_min = 0;
        timeinfo->tm_sec = 0;
        timeinfo->tm_isdst = false;
        t = mktime(timeinfo);
    }
    catch (...) {
        //throw;
        // do not throw any exceptions in library. 
        cout << "Error occurred when convert " + strDate + " to time_t!" << endl;
        t = 0;  // reset to 0 for convenient comparison
    }

    return t;
}

time_t utilsTime::ConvertToTime2(string const &strDate, const char *format, bool includeHour) {
    time_t t(0);
    if (utilsString::StringMatch(strDate, "")) {
        return t;
    }
    int yr;
    int mn;
    int dy;
    int hr = 0;
    int m = 0;
    int s = 0;

    try {
        if (includeHour) {
            stringscanf(strDate.c_str(), format, &yr, &mn, &dy, &hr, &m, &s);
        } else {
            stringscanf(strDate.c_str(), format, &yr, &mn, &dy);
        }

        struct tm timeinfo;
        timeinfo.tm_year = yr - 1900;
        timeinfo.tm_mon = mn - 1;
        timeinfo.tm_mday = dy;
        timeinfo.tm_hour = hr;
        timeinfo.tm_min = m;
        timeinfo.tm_sec = s;
        timeinfo.tm_isdst = false;
        t = mktime(&timeinfo);
    }
    catch (...) {
        cout << "Error in ConvertToTime2!" << endl;
        t = 0;
        //throw;
    }

    return t;
}

time_t utilsTime::ConvertYMDToTime(int &year, int &month, int &day) {
    time_t t(0);
    try {
        struct tm timeinfo;
        timeinfo.tm_year = year - 1900;
        timeinfo.tm_mon = month - 1;
        timeinfo.tm_mday = day;
        timeinfo.tm_isdst = false;
        t = mktime(&timeinfo);
    }
    catch (...) {
        cout << "Error in ConvertYMDToTime!" << endl;
        t = 0;
        //throw;
    }
    return t;
}

int utilsTime::GetDateInfoFromTimet(time_t *t, int *year, int *month, int *day) {
    struct tm dateInfo;
#ifdef windows
    localtime_s(&dateInfo, t);
#else
    localtime_r(t, &dateInfo);
#endif /* windows */
    if (dateInfo.tm_isdst > 0) {
        dateInfo.tm_hour -= 1;
    }

    char dateString[30];
    strftime(dateString, 30, "%Y-%m-%d %X", &dateInfo);
    int hour, min, sec;
    stringscanf(dateString, "%4d-%2d-%2d %2d:%2d:%2d", year, month, day, &hour, &min, &sec);
    return 0;
}

void utilsTime::LocalTime(time_t date, struct tm *t) {
#ifdef windows
    localtime_s(t, &date);
#else
    localtime_r(&date, t);
#endif /* windows */
}

/************ utilsString ****************/
string utilsString::GetUpper(const string &str) {
    string strTmp1 = string(str);
    for (int j = 0; j < (int) strTmp1.length(); j++) strTmp1[j] = (char) toupper(strTmp1[j]);
    return strTmp1;
}

void utilsString::TrimSpaces(string &str) {
    // Find the first character position after excluding leading blank spaces
    size_t startpos = str.find_first_not_of(" \t");
    // Find the first character position from reverse
    size_t endpos = str.find_last_not_of(" \t");
    // if all spaces or empty return an empty string
    if ((string::npos == startpos) || (string::npos == endpos)) {
        str = "";
    } else {
        str = str.substr(startpos, endpos - startpos + 1);
    }
}

vector<string> utilsString::SplitString(const string &item, char delimiter) {
    istringstream iss(item);
    vector<string> tokens;

    std::string field;
    while (std::getline(iss, field, delimiter)) {
        tokens.emplace_back(field);
    }
    vector<string>(tokens).swap(tokens);
    // tokens.shrink_to_fit(); // C++11, which may not supported by compiler
    return tokens;
}

vector<int> utilsString::SplitStringForInt(const string &item, char delimiter) {
    vector<string> valueStrs = utilsString::SplitString(item, delimiter);
    vector<int> values;
    for (auto it = valueStrs.begin(); it != valueStrs.end(); it++) {
        values.emplace_back(atoi((*it).c_str()));
    }
    vector<int>(values).swap(values);
    return values;
}

vector<float> utilsString::SplitStringForFloat(const string &item, char delimiter) {
    vector<string> valueStrs = utilsString::SplitString(item, delimiter);
    vector<float> values;
    for (auto it = valueStrs.begin(); it != valueStrs.end(); it++) {
        values.emplace_back((float) atof((*it).c_str()));
    }
    vector<float>(values).swap(values);
    return values;
}

vector<string> utilsString::SplitString(const string &item) {
    istringstream iss(item);
    vector<string> tokens;

    std::string field;
    iss >> field;
    while (!iss.eof()) {
        tokens.emplace_back(field);
        iss >> field;
    }
    tokens.emplace_back(field);

    return tokens;
}

bool utilsString::StringMatch(const char *a, const char *b) {
    return strcasecmp(a, b) == 0;
}

bool utilsString::StringMatch(const string &text1, const string &text2) {
    // convert the key to UPPERCASE for comparison
    string strTmp1 = utilsString::GetUpper(text1);
    string strTmp2 = utilsString::GetUpper(text2);
    return (strTmp1 == strTmp2);
}

string &utilsString::trim(string &s) {
    if (s.empty()) {
        return s;
    }
    s.erase(0, s.find_first_not_of(" \n\r\t"));
    return s.erase(s.find_last_not_of(" \n\r\t") + 1);
}

/************ utilsArray *****************/
void utilsArray::Output1DArrayToTxtFile(int n, CFLOATPTR data, const char *filename) {
    ofstream ofs(filename);
    for (int i = 0; i < n; ++i) {
        ofs << data[i] << "\n";
    }
    ofs.close();
}

void utilsArray::Output2DArrayToTxtFile(int nRows, int nCols, const CFLOATPTR *data, const char *filename) {
    ofstream ofs(filename);
    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            ofs << data[i][j] << "\t";
        }
        ofs << "\n";
    }
    ofs.close();
}

void utilsArray::Read1DArrayFromTxtFile(const char *filename, int &nRows, float *&data) {
    ifstream ifs(filename);
    string tmp;
    ifs >> tmp >> nRows;
    data = new float[nRows];
    for (int i = 0; i < nRows; i++) {
        ifs >> data[i];
    }
    ifs.close();
}

void utilsArray::Read2DArrayFromTxtFile(const char *filename, int &nRows, float **&data) {
    ifstream ifs(filename);
    string tmp;
    ifs >> tmp >> nRows;
    data = new float *[nRows];
    int n;
    for (int i = 0; i < nRows; i++) {
        ifs >> n;
        data[i] = new float[n + 1];
        data[i][0] = (float) n;
        for (int j = 1; j <= n; j++) {
            ifs >> data[i][j];
        }
    }
    ifs.close();
}

void utilsArray::Read2DArrayFromString(const char *s, int &nRows, float **&data) {
    istringstream ifs(s);

    string tmp;
    ifs >> tmp >> nRows;
    data = new float *[nRows];
    int n;
    for (int i = 0; i < nRows; i++) {
        ifs >> n;
        data[i] = new float[n + 1];
        data[i][0] = (float) n;
        for (int j = 1; j <= n; j++) {
            ifs >> data[i][j];
        }
    }
}

/************ utilsMath ******************/
float utilsMath::Expo(float xx, float upper /* = 20.f */, float lower /* = -20.f */) {
    if (xx < lower) xx = lower;
    if (xx > upper) xx = upper;
    return exp(xx);
}

float utilsMath::Power(float a, float n) {
    if (a >= 0.f) {
        return pow(a, n);
    } else {
        return -pow(-a, n);
    }
}

/************ utilsFileIO ******************/
#ifndef windows

int utilsFileIO::copyfile_unix(const char *srcfile, const char *dstfile) {
    struct stat file;
    if (stat(srcfile, &file) == -1) {
        printf("Can not get info of file %s in function: copyfile_linux.", srcfile);
        return -1;
    }

    int f1 = open(srcfile, O_RDONLY);
    if (f1 == -1) {
        printf("Can not read file %s in function: copyfile_linux.", srcfile);
        return -1;
    }

    int f2 = creat(dstfile, file.st_mode);
    if (f2 == -1) {
        printf("Can not create file %s in function: copyfile_linux.", srcfile);
        close(f1);
        return -1;
    }

    char buf[200] = "";
    int size = 0;
    while ((size = read(f1, buf, 200)) != 0) {
        if (write(f2, buf, size) != size) {
            printf("Write error to file: %s function: copyfile_linux.", dstfile);
            close(f1);
            close(f2);
            return -1;
        }
    }

    close(f1);
    close(f2);

    return 0;
}

#endif /* windows */

bool utilsFileIO::FileExists(string const &FileName) {

#ifdef windows
    struct _finddata_t fdt;
    intptr_t ptr = _findfirst(FileName.c_str(), &fdt);
    bool found = (ptr != -1);
    _findclose(ptr);
    return found;
#else
    if (access(FileName.c_str(), F_OK) == 0) {
        return true;
    } else {
        return false;
    }
#endif /* windows */
}

bool utilsFileIO::PathExists(string const &fullpath) {
    string abspath = utilsFileIO::GetAbsolutePath(fullpath);
    const char *path = abspath.c_str();
    bool isExists;
#ifdef windows
    struct _stat fileStat;
    isExists = (_stat(path, &fileStat) == 0) && (fileStat.st_mode & _S_IFDIR);
#else
    struct stat fileStat;
    isExists = (stat(path, &fileStat) == 0) && S_ISDIR(fileStat.st_mode);
#endif /* windows */
    return isExists;
}

int utilsFileIO::DeleteExistedFile(const string &filepath) {
    string abspath = utilsFileIO::GetAbsolutePath(filepath);
    if (utilsFileIO::FileExists(abspath)) {
        return remove(abspath.c_str());
    } else {
        return -1;
    }
}

int utilsFileIO::FindFiles(const char *lpPath, const char *expression, vector<string> &vecFiles) {
    string abspath = utilsFileIO::GetAbsolutePath(lpPath);
    const char* newlpPath = abspath.c_str();
#ifdef windows
    char szFind[MAX_PATH];
    stringcpy(szFind, newlpPath);
    stringcat(szFind, SEP);
    stringcat(szFind, expression);

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = ::FindFirstFile(szFind, &findFileData);
    if (INVALID_HANDLE_VALUE == hFind) {
        return -1;
    }
    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue;
        }

        char fullpath[MAX_PATH];
        stringcpy(fullpath, newlpPath);
        stringcat(fullpath, SEP);
        stringcat(fullpath, findFileData.cFileName);

        vecFiles.emplace_back(fullpath);

    } while (::FindNextFile(hFind, &findFileData));
#else
    DIR *dir = opendir(newlpPath);
    //cout<<"Find existed files ..."<<endl;
    if (dir) {
        struct dirent *hFile;
        errno = 0;
        while ((hFile = readdir(dir)) != nullptr) {
            if (!strcmp(hFile->d_name, ".")) continue;
            if (!strcmp(hFile->d_name, "..")) continue;

            // in linux hidden files all start with '.'
            if (hFile->d_name[0] == '.') continue;

            string filename(hFile->d_name);
            // cout << filename<<endl;
            string ext = utilsFileIO::GetSuffix(filename);
            // cout << ext << "\t" << expression << endl;
            string strexpression = string(expression);
            if (utilsString::StringMatch(ext.c_str(), expression) || strexpression.find(ext) != string::npos
                || utilsString::StringMatch(expression, ".*")
                || utilsString::StringMatch(expression, "*.*")) {
                ostringstream oss;
                oss << newlpPath << SEP << filename;
                cout << oss.str() << endl;
                vecFiles.emplace_back(oss.str());
            }
        }
        closedir(dir);
    }
#endif /* windows */
    return 0;
}

bool utilsFileIO::DirectoryExists(const string &dirpath) {
    string abspath = utilsFileIO::GetAbsolutePath(dirpath);
#ifdef windows
    if (::GetFileAttributes(abspath.c_str()) == INVALID_FILE_ATTRIBUTES) {
#else
        if (access(abspath.c_str(), F_OK) != 0) {
#endif /* windows */
        return false;
    } else {
        return true;
    }
}

bool utilsFileIO::CleanDirectory(const string &dirpath) {
    string abspath = utilsFileIO::GetAbsolutePath(dirpath);
    try {
        if (utilsFileIO::DirectoryExists(abspath)) { /// empty the directory
            vector<string> existedFiles;
            utilsFileIO::FindFiles(abspath.c_str(), "*.*", existedFiles);
            for (auto it = existedFiles.begin(); it != existedFiles.end(); ++it) {
                remove((*it).c_str());
            }
        } else { /// create new directory
#ifdef windows
            LPSECURITY_ATTRIBUTES att = nullptr;
            ::CreateDirectory(abspath.c_str(), att);
#else
            mkdir(abspath.c_str(), 0777);
#endif /* windows */
        }
        return true;
    }
    catch (...) {
        cout << "Create or clean directory: " << abspath << " failed!" << endl;
        return false;
    }
}

bool utilsFileIO::DeleteDirectory(const string &dirpath, bool delSubdirs/* = true */) {
    string abspath = utilsFileIO::GetAbsolutePath(dirpath);
    if (!utilsFileIO::DirectoryExists(abspath)) return true;
#ifdef windows
    bool            bSubdirectory = false;       // Flag, indicating whether
                                                 //   subdirectories have been found
    HANDLE          hFile;                       // Handle to directory
    std::string     strFilePath;                 // Filepath
    std::string     strPattern;                  // Pattern
    WIN32_FIND_DATA FileInformation;             // File information

    strPattern = abspath + SEP + "*.*";
    hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
    if(hFile != INVALID_HANDLE_VALUE) {
        do {
            if(FileInformation.cFileName[0] != '.') {
                strFilePath.erase();
                strFilePath = abspath + SEP + FileInformation.cFileName;
                if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    if(delSubdirs) {
                        // Delete subdirectory
                        bool iRC = utilsFileIO::DeleteDirectory(strFilePath, delSubdirs);
                        if(!iRC) return false;
                    } else bSubdirectory = true;
                }
                else {
                    // Set file attributes
                    if(::SetFileAttributes(strFilePath.c_str(), FILE_ATTRIBUTE_NORMAL) == FALSE) {
                        // ::GetLastError();
                        return false;
                    }
                    // Delete file
                    if(::DeleteFile(strFilePath.c_str()) == FALSE) {
                        // ::GetLastError();
                        return false;
                    }
                }
            }
        } while(::FindNextFile(hFile, &FileInformation) == TRUE);

        // Close handle
        ::FindClose(hFile);

        DWORD dwError = ::GetLastError();
        if(dwError != ERROR_NO_MORE_FILES) {
            // dwError;
            return false;
        }
        else {
            if(!bSubdirectory) {
                // Set directory attributes
                if(::SetFileAttributes(abspath.c_str(), FILE_ATTRIBUTE_NORMAL) == FALSE) {
                    // ::GetLastError();
                    return false;
                }
                // Delete directory
                if(::RemoveDirectory(abspath.c_str()) == FALSE) {
                    // ::GetLastError();
                    return false;
                }
            }
        }
    }
    return true;
#else
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];

    dir = opendir(abspath.c_str());
    if (dir == NULL) {
        perror("Error opendir()");
        return true;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            snprintf(path, (size_t) PATH_MAX, "%s/%s", abspath.c_str(), entry->d_name);
            if (entry->d_type == DT_DIR && delSubdirs) {
                utilsFileIO::DeleteDirectory(path, delSubdirs);
            }
            printf("Deleting: %s\n", path);
            remove(path);
        }
    }
    closedir(dir);
    printf("Deleting: %s\n", abspath.c_str());
    remove(abspath.c_str());
    return true;
#endif /* windows */
}

string utilsFileIO::GetAppPath() {
    string RootPath;
#ifdef windows
    TCHAR buffer[PATH_MAX];
    GetModuleFileName(nullptr, buffer, PATH_MAX);
    RootPath = string((char *) buffer);
#elif (defined macos) || (defined macosold)
    /// http://stackoverflow.com/a/8149380/4837280
    int ret;
    pid_t pid;
    char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
    pid = getpid();
    ret = proc_pidpath (pid, pathbuf, sizeof(pathbuf));
    if ( ret <= 0 ) {
        fprintf(stderr, "PID %d: proc_pidpath ();\n", pid);
        fprintf(stderr, "    %s\n", strerror(errno));
    } else {
        printf("proc %d: %s\n", pid, pathbuf);
    }
    RootPath = pathbuf;
#else /* other linux/unix-like OS */
    static char buf[PATH_MAX];
    int rslt = readlink("/proc/self/exe", buf, PATH_MAX);
    if (rslt < 0 || rslt >= PATH_MAX) {
        buf[0] = '\0';
    } else {
        buf[rslt] = '\0';
    }
    RootPath = buf;
#endif /* windows */
    basic_string<char>::size_type idx = RootPath.find_last_of(SEP);
    RootPath = RootPath.substr(0, idx + 1);

    return RootPath;
}

string utilsFileIO::GetAbsolutePath(string const &fullFileName) {
#ifdef windows
    TCHAR full_path[MAX_PATH];
    GetFullPathName(fullFileName.c_str(), MAX_PATH, full_path, nullptr);
#else
    char full_path[PATH_MAX];
    realpath(fullFileName.c_str(), full_path);
#endif /* windows */
    return string(full_path);
}

string utilsFileIO::GetCoreFileName(string const &fullFileName) {
    string abspath = utilsFileIO::GetAbsolutePath(fullFileName);
    string::size_type start = abspath.find_last_of(SEP);
    string::size_type end = abspath.find_last_of(".");
    if (end == string::npos) {
        end = abspath.length();
    }
    return abspath.substr(start + 1, end - start - 1);
}

string utilsFileIO::GetSuffix(string const &fullFileName) {
    string abspath = utilsFileIO::GetAbsolutePath(fullFileName);
    vector<string> tokens = utilsString::SplitString(abspath, '.');
    if (tokens.size() >= 2) {
        return tokens[tokens.size() - 1];
    } else {
        return "";
    }
}

string utilsFileIO::ReplaceSuffix(string const &fullFileName, string const &newSuffix) {
    string filedir = utilsFileIO::GetPathFromFullName(fullFileName);
    string corename = utilsFileIO::GetCoreFileName(fullFileName);
    string oldSuffix = utilsFileIO::GetSuffix(fullFileName);
    if (filedir.empty() || oldSuffix.empty()) return "";
    return filedir + corename + "." + newSuffix;
}

string utilsFileIO::GetPathFromFullName(string const &fullFileName) {
    string abspath = utilsFileIO::GetAbsolutePath(fullFileName);
    string::size_type i = abspath.find_last_of(SEP);
    if (i == string::npos) {
        cout << "No valid path in " << fullFileName << ", please check!" << endl;
        return "";
    }
    return abspath.substr(0, i + 1);
}

bool utilsFileIO::LoadPlainTextFile(const string &filepath, vector<string> &contentStrs) {
    string abspath = utilsFileIO::GetAbsolutePath(filepath);
    bool bStatus = false;
    ifstream myfile;
    string line;
    try {
        // open the file
        myfile.open(abspath.c_str(), ios::in);
        if (myfile.is_open()) {
            while (!myfile.eof()) {
                if (myfile.good()) {
                    getline(myfile, line);
                    line = utilsString::trim(line);
                    if ((!line.empty()) && (line[0] != '#')) // ignore comments and empty lines
                    {
                        contentStrs.push_back(line);
                        bStatus = true; // consider this a success
                    }
                }
            }
            bStatus = true;
            myfile.close();
            vector<string>(contentStrs).swap(contentStrs);
            // contentStrs.shrink_to_fit();
        }
    }
    catch (...) {
        myfile.close();
        cout << "Load plain text file: " << filepath << " failed!" << endl;
    }
    return bStatus;
}

/************ utils ******************/
bool utils::isIPAddress(const char *ip) {
    const char *pChar;
    bool rv = true;
    int tmp1, tmp2, tmp3, tmp4, i;

    while (true) {
        i = stringscanf(ip, "%d.%d.%d.%d", &tmp1, &tmp2, &tmp3, &tmp4);
        if (i != 4) {
            rv = false;
            //cout << "IP Address format is not correct!" << endl;
            break;
        }
        if ((tmp1 > 255) || (tmp2 > 255) || (tmp3 > 255) || (tmp4 > 255) || (tmp1 < 0) || (tmp2 < 0) || (tmp3 < 0) ||
            (tmp4 < 0)) {
            rv = false;
            //cout << "IP Address format is not correct!" << endl;
            break;
        }
        for (pChar = ip; *pChar != 0; pChar++) {
            if ((*pChar != '.') && ((*pChar < '0') || (*pChar > '9'))) {
                rv = false;
                //cout << "IP Address format is not correct!" << endl;
                break;
            }
        }
        break;
    }
    return rv;
}

void utils::Log(string msg, string logpath /* = "debugInfo.log" */) {
    struct tm timeptr;
    time_t now;
    char buffer[32];
    time(&now);
#ifdef windows
    localtime_s(&timeptr, &now);
    asctime_s(buffer, 32, &timeptr);
#else
    localtime_r(&now, &timeptr);
    asctime_r(&timeptr, buffer);
#endif /* windows */
    string timestamp = buffer;
    timestamp = timestamp.substr(0, timestamp.length() - 1);
    fstream fs(logpath.c_str(), ios::app);
    if (fs.is_open()) {
        fs << timestamp;
        fs << ": ";
        fs << msg;
        fs << endl;
        fs.close();
    }
}

int utils::GetAvailableThreadNum() {
#ifdef windows
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#endif /* windows */
#ifdef linux
    return (int) sysconf(_SC_NPROCESSORS_ONLN);
#endif /* linux */
#ifdef macos
    // macOS X 10.5 and later
    return (int) sysconf(_SC_NPROCESSORS_ONLN);
#endif /* macOS X 10.5 and later */
#ifdef macosold
    // macOS X 10.0 - 10.4
    int mib[4];
    int numCPU;
    std::size_t len = sizeof(numCPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &numCPU, &len, nullptr, 0);

    if (numCPU < 1)
    {
        mib[1] = HW_NCPU;
        sysctl(mib, 2, &numCPU, &len, nullptr, 0);
        if (numCPU < 1)
            numCPU = 1;
    }
    return numCPU;
#endif /* macOS X 10.0 - 10.4 */
}

void utils::SetDefaultOpenMPThread() {
#ifdef SUPPORT_OMP
    // omp thread have not been set
    if (omp_get_num_threads() <= 1){
        // set one half of the available threads as default
        omp_set_num_threads(GetAvailableThreadNum() / 2);
    }
#endif /* SUPPORT_OMP */
    /// do nothing if OMP is not supported
}

void utils::SetOpenMPThread(int n) {
#ifdef SUPPORT_OMP
    omp_set_num_threads(n);
#endif /* SUPPORT_OMP */
    /// do nothing if OMP is not supported
}

void utils::StatusMessage(const char *msg) {
    /// Just for debugging ///
#ifdef _DEBUG
    cout << msg << endl;
#endif /* DEBUG */
}

#endif /* CLS_UTILS */
