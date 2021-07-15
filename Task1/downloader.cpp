#include "downloader.h"

Downloader::Downloader() {
    CURLcode code = curl_global_init(CURL_GLOBAL_ALL);
    m_handle = new handle_t{curl_easy_init(), &curl_easy_cleanup};
}

Downloader::Downloader(const std::string& url, const std::string& path_to_save) : Downloader(){
    get_file(url, path_to_save);
}

Downloader::~Downloader() {
    delete m_handle;
    delete m_file;
    curl_global_cleanup();
}

void Downloader::get_file(const std::string& url, const std::string& path_to_save){
    m_url = url;
    m_file_name = path_to_save;

    m_file = new file_t(std::fopen(m_file_name.c_str(), "w+"),[](FILE* f) { std::fclose(f); });

    curl_easy_setopt(m_handle->get(), CURLOPT_URL, m_url.c_str());
    curl_easy_setopt(m_handle->get(), CURLOPT_WRITEFUNCTION, Downloader::write_data);
    curl_easy_setopt(m_handle->get(), CURLOPT_WRITEDATA, m_file->get());

    CURLcode res = curl_easy_perform(m_handle->get());
    std::string c_res = curl_easy_strerror(res);
    std::cout << "[Downloader] " << c_res << std::endl;

    curl_easy_reset(m_handle->get());
}

size_t Downloader::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}