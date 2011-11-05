#ifndef CONFIG_H
#define CONFIG_H

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>

namespace Config
{
    class ConfigCenter;
    typedef boost::shared_ptr<ConfigCenter> ConfigCenterPtr;
    class ConfigCenter
    {
    public:
        ~ConfigCenter();
        /**
         * static ConfigCenter* instance()
         * Description: the instance will load the xml file "config.xml".
         *              if the other files should be used, please call loadConfig in main.
         *
         *              The instance is a smart pointer, it will live longer than or as long as the session does.
         *              The instance will be replaced while loadConfig is called.
         *              Once the intance is got, the content of instance() will not changed any more.
         *
         *              There is no lock while accessing the instance's get/set method.
         *              The lock lies in the instance() method.
         *              So the performance reason, the user may consider saving the instance to a session. 
         */
        static ConfigCenterPtr instance();
        /**
         * static void loadConfig(const std::string theInputXmlFile)
         * Description: it will safely replace the ConfigCenterPtr::instance
         *              with the data stored in the input xml file.
         */
        static void loadConfig(const std::string theInputXmlFile = "config.xml");

        template<typename ValueType>
        inline ValueType get(const std::string& theKey, const ValueType theDefault)
        {
            return configDataM.get<ValueType>(theKey, theDefault);
        }
        template<typename ValueType>
        inline void set(const std::string& theKey, const ValueType& theValue)
        {
            configDataM.put(theKey, theValue);
        }

        int loadXml(const std::string theXmlPath);
        int saveXml(const std::string theXmlPath);

    private:
        static ConfigCenterPtr configCenterM;
        static boost::shared_mutex configCenterMutexM;

        ConfigCenter();
        /*config data implement*/
        boost::property_tree::ptree configDataM;
    };
}

#endif /* CONFIG_H */

