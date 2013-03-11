//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

// @author Thorsten Jacobi
// @brief CoolstreamingSource based on CoolstreamingBase
// @ingroup mesh
// @ingroup coolstreaming

#include "CoolstreamingSource.h"

Define_Module(CoolstreamingSource);

CoolstreamingSource::CoolstreamingSource() {
    // TODO Auto-generated constructor stub

}

CoolstreamingSource::~CoolstreamingSource() {
    // TODO Auto-generated destructor stub
}

void CoolstreamingSource::initialize(int stage){
    if (stage != 3)
        return;

    initBase();
}
