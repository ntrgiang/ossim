//
// Copyright (C) 2004 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#ifndef __INETMODULEACCESS_H__
#define __INETMODULEACCESS_H__

#include "ModuleAccess.h"

/**
 * Finds and returns the pointer to a module of type T and name N.
 * Uses findModuleWherever(). See usage e.g. at RoutingTableAccess.
 */
template<typename T>
class INET_API INetModuleAccess
/* cannot derive from ModuleAccess, as get / getIfExists aren't virtual*/
{
     // Note: MSVC 6.0 doesn't like const char *N as template parameter,
     // so we have to pass it via the ctor...
  private:
    const char *name;
    T *p;
  public:
    INetModuleAccess(const char *n) {name = n; p=NULL;}
    T *get()
    {
        if (!p)
        {
        	// INET 2006
            // cModule *m = findModuleWherever(name, opp_typename(typeid(T)), simulation.getContextModule());
        	cModule *m = findModuleWherever(name, simulation.getContextModule());

            if (!m) opp_error("Module (%s)%s not found",opp_typename(typeid(T)),name);
            p = check_and_cast<T*>(m);
        }
        return p;
    }

    T *getIfExists()
    {
        if (!p)
        {
            cModule *m = findModuleSomewhereUp(name, simulation.getContextModule());
            p = dynamic_cast<T*>(m);
        }
        return p;
    }
};

#endif

