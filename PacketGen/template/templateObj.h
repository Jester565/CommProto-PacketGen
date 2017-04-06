/**
^MsgComment^

Copyright (C) 2016-2017 Northrup Grumman Collaboration Project.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(At your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef ^Define^
#define ^Define^


#include <CommProto/commproto.h>
#include <CommProto/serialization/objserialization.h>
^Include^

namespace ngcp {

  struct ^Name^ : INHERITS_OBJSERIALIZABLE {
    /**
    Creates an instance
    */
    ^Constructor^

    /**
    Pack data into the stream for sending out.
    */
    void Input(comnet::ObjectStream &obj) override {
      ^PackBody^
    }


    /**
    Unpack data back into this packet when receiving data.
    */
    void Output(comnet::ObjectStream &obj) override {
      ^UnpackBody^
    }

    /**
    Tells CommProtocol how to recreate the ^Name^ packet
    when receiving data.
    */
    comnet::serialization::ObjSerializable* Create() override {
      return new ^CreateConstruct^;
    }

    /**
    Data.
    */
    ^Data^
  };
} // ngcp
#endif // ^Define^
