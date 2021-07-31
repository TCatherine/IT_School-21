#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <iostream>
#include <cstring>
#include <memory>
#include <functional>

#include <curl/curl.h>

 class CurlGlobalInit
 {
    protected:
    CurlGlobalInit() { curl_global_init(CURL_GLOBAL_ALL);  }
    ~CurlGlobalInit() { curl_global_cleanup(); }
};

class Downloader {
    public:
    Downloader();
    Downloader(const std::string& url,const std::string& path_to_save);
    ~Downloader();

    void get_file(const std::string& url,const std::string& path_to_save);

    private:
		static CurlGlobalInit m_curl_global_init;
    using handle_t = std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> ;
    using file_t = std::unique_ptr<FILE, std::function<void(FILE*)>>;

    std::string m_url = "";
    std::string m_file_name = "";

    handle_t m_handle{curl_easy_init(), &curl_easy_cleanup};

    static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) ;
};

#endif //DOWNLOADER_H