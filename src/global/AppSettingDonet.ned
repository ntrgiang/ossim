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

package simpleoverlay.global;

simple AppSettingDonet extends AppSetting
{
    parameters:
        @class(AppSettingDonet);

        int videoStreamBitRate  @unit(bps)  = default(100kbps);
        int chunkSize           @unit(byte) = default(1250B);
        int bufferMapSize       @unit(s)    = default(2s);
        
        double interval_bufferMap @unit(s) = default(0s);
}
