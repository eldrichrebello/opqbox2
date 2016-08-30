#ifndef CORESETTINGS_HPP
#define CORESETTINGS_HPP

#include <string>
#include <map>
#include <mutex>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

namespace opq {
/**
 * @brief The Settings class synchronized singleton key value pair database.
 */
    typedef boost::variant<uint64_t, float, int, std::string, bool> OPQSetting;

    class Settings {
    public:
        /**
         * @brief Instance get an instance of the settings manager.
         * @return
         */
        static Settings *Instance();

        /**
         * @brief setSetting set a key value pair.
         * @param key key.
         * @param value value.
         * @return false if the key is not present otherwise true.
         */
        bool setSetting(std::string key, OPQSetting value);

        /**
         * @brief getSetting get the value for a key.
         * @param key key.
         * @return value corresponding to the key.
         */
        OPQSetting getSetting(std::string key);

        /**
         * @brief isSet checks if the key is present.
         * @param key key.
         * @return true is the key is set.
         */
        bool isSet(std::string key);

        /**
         * @brief erase erases a key from the database.
         * @param key key.
         * @return setting that was erased.
         */
        OPQSetting erase(std::string key);

        /**
         * @brief loadFromFile load the database from file.
         * @param filename filename.
         * @return true on success, false otherwise.
         */
        bool loadFromFile(std::string filename);

        /**
         * @brief saveToFile save the database to a file. Will overwrite the file.
         * @param filename filename.
         * @return true on success, false otherwise.
         */
        bool saveToFile(std::string filename);

        /**
         * @brief clear clear everything.
         */
        void clear();

        uint64_t getUint64(std::string key);
        float getFloat(std::string key);
        int getInt(std::string key);
        std::string getString(std::string key);
        bool getBool(std::string key);

        int onChangeCallback(std::string key, std::function<void(OPQSetting)>);
        bool removeChangeCallback(std::string key, int id);

    private:
        Settings();

        Settings(Settings const &);

        Settings &operator=(Settings const &);

        static Settings *_instance;

        typedef struct{
            OPQSetting setting;
            std::map<int, std::function<void(OPQSetting) > > callbacks;
            int lastId;
        }SettingStruct;

        std::map<std::string, SettingStruct> _settings;


        std::mutex _mutex;
    };
}
#endif // CORESETTINGS_HPP
