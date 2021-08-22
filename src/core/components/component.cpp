#include "component.hpp"

#include <utility>  // move

/*
 <components>
   <component name="Movable">
     <properties>
       <property name="PositionX" type="float" value="0" />
       <property name="PositionY" type="float" value="0" />
       <property name="VelocityX" type="float" value="0" />
       <property name="VelocityY" type="float" value="0" />
     </properties>
   </component>
 </components>

 "components": [
   {
     "name": "Movable",
     "properties": [
       {
         "name": "PositionX",
         "type": "float",
         "value": 0
       },
       {
         "name": "PositionY",
         "type": "float",
         "value": 0
       },
       {
         "name": "VelocityX",
         "type": "float",
         "value": 0
       },
       {
         "name": "VelocityY",
         "type": "float",
         "value": 0
       }
     ]
   }
 ]
 */

namespace Tactile {

Component::Component(std::string type) : mType{std::move(type)}
{}

}  // namespace Tactile