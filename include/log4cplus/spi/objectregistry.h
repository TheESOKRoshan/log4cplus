// Module:  Log4CPLUS
// File:    objectregistry.h
// Created: 3/2003
// Author:  Tad E. Smith
//
//
// Copyright (C) The Apache Software Foundation. All rights reserved.
//
// This software is published under the terms of the Apache Software
// License version 1.1, a copy of which has been included with this
// distribution in the LICENSE.APL file.
//

/** @file */

#ifndef LOG4CPLUS_SPI_OBJECT_REGISTRY_HEADER_
#define LOG4CPLUS_SPI_OBJECT_REGISTRY_HEADER_

#include <log4cplus/config.h>
#include <log4cplus/helpers/threads.h>
#include <map>
#include <memory>
#include <string>
#include <vector>


namespace log4cplus {
    namespace spi {

        /**
         * This is the base class used to implement the functionality required
         * by the ObjectRegistry template class.
         */
        class ObjectRegistryBase {
        public:
          // public methods
            /**
             * Tests to see whether or not an object is bound in the
             * registry as <code>name</code>.
             */
            bool exists(const std::string& name) const;

            /**
             * Returns the names of all registered objects.
             */
            std::vector<std::string> getAllNames() const;

        protected:
          // Ctor and Dtor
            ObjectRegistryBase();
            virtual ~ObjectRegistryBase();

          // protected methods
            /**
             * Used to enter an object into the registry.  (The registry now
             * owns <code>object</code>.)
             */
            bool putVal(const std::string& name, void* object);

            /**
             * Used to retrieve an object from the registry.  (The registry
             * owns the returned pointer.)
             */
            void* getVal(const std::string& name) const;

            /**
             * Deletes <code>object</code>.
             */
            virtual void deleteObject(void *object) const = 0;

            /**
             * Deletes all objects from this registry.
             */
            virtual void clear();

          // Types
            typedef std::map<std::string, void*> ObjectMap;

          // Data
            LOG4CPLUS_MUTEX_PTR_DECLARE mutex;
            ObjectMap data;
        };



        /**
         * This template class is used as a "Object Registry".  Objects are
         * "entered" into the registry with a "name" using the <code>put()</code>
         * method.  (The registry then owns the object.)  These object can
         * then be retrieved using the <code>get()</code> method.
         * <p>
         * <b>Note:</b>  This class is Thread-safe.
         */
        template<class T>
        class ObjectRegistry : ObjectRegistryBase {
        public:
            virtual ~ObjectRegistry() {
                clear();
            }

          // public methods
            /**
             * Used to enter an object into the registry.  (The registry now
             * owns <code>object</code>.)
             */
            bool put(const std::string& name, std::auto_ptr<T> object) {
                return putVal(name, object.release());
            }

            /**
             * Used to retrieve an object from the registry.  (The registry
             * owns the returned pointer.)
             */
            T* get(const std::string& name) const {
                return static_cast<T*>(getVal(name));
            }

        protected:
            virtual void deleteObject(void *object) const {
                delete static_cast<T*>(object);
            }
        };

    }
}


#endif // LOG4CPLUS_SPI_OBJECT_REGISTRY_HEADER_

