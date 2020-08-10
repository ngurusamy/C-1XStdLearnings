#include <mutex>
#include <assert.h>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include <stdio.h>

// 
// Requirements:
//  1: the bitmap could be used by multiple thread safely.(std::shared_ptr could?)
//  2: cache the bitmap and do not always increase memeory
//@NotThreadSfe
struct Bitmap {
    public:
        Bitmap(const std::string& filePath) { 
            filePath_ = filePath;
            printf("foo %x ctor %s\n", this, filePath_.c_str());
        }
        ~Bitmap() {
            printf("foo %x dtor %s\n", this, filePath_.c_str());
        }
        std::string filePath_;
};

//@ThreadSafe
struct BitmapCache {
    public:
        static std::shared_ptr<Bitmap> loadBitmap(const std::string& filePath) {
            mutex_.lock();

            //whether in the cache
            auto iter = cache_.find(filePath);
            if (iter != cache_.end()) {
                if ((*iter).second) {
                    return (*iter).second;
                } else {
                    std::shared_ptr<Bitmap> newPtr(new Bitmap(filePath));
                    (*iter).second = newPtr;
                    return newPtr;
                }
            }

            //try remove unused elements if possible
            if (cache_.size() >= kSlotThreshold) {
                std::unordered_map<std::string,std::shared_ptr<Bitmap>>::iterator delIter = cache_.end();
                for (auto iter = cache_.begin(); iter != cache_.end(); ++iter) {
                    auto& item = *iter;
                    if (item.second && item.second.use_count() == 1) {
                        delIter = iter;
                        break;
                    }
                }
                if (cache_.end() != delIter) {
                    (*delIter).second.reset();
                    cache_.erase(delIter);
                }
            }

            //create new and insert to the cache
            std::shared_ptr<Bitmap> newPtr(new Bitmap(filePath));
            cache_.insert({filePath, newPtr});
            mutex_.unlock();
            return newPtr;
        }
    private:
        static const int kSlotThreshold = 20;
        static std::mutex mutex_;
        static std::unordered_map<std::string,std::shared_ptr<Bitmap>> cache_;
};

/* static */
std::unordered_map<std::string,std::shared_ptr<Bitmap>> BitmapCache::cache_;

/* static */
std::mutex BitmapCache::mutex_;

int main()
{
    //test for remove useless element
    char buff[200] = {0};
    std::vector<std::shared_ptr<Bitmap>> bmpVec(20);
    for (int i = 0; i < 20; ++i) {
        sprintf_s(buff, 200, "c:\\haha%d.bmp", i);
        bmpVec[i] = BitmapCache::loadBitmap(buff);
    }
    bmpVec[3].reset();
    std::shared_ptr<Bitmap> newBmp = BitmapCache::loadBitmap("c:\\new.bmp");

    //test for multiple threading...(to be implemenetd)
    return 0;
}
