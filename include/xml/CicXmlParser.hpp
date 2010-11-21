#ifndef CIC_XML_PARSER_HPP
#define CIC_XML_PARSER_HPP

#include <sdr_simulator/xml/IXmlParser.hpp>

class CicXmlParser : public xml::IXmlParser
{
   const std::string name_;
   xml::NodeMap map_;

   void Insert( ticpp::Node* node, const std::string& value )
   {
      map_.insert
         ( 
          std::pair< std::string, std::string >( 
             value,
             node->FirstChildElement( value )->GetText() 
             )
         );
   }

   public:

   CicXmlParser(): name_("cic"){}

   const xml::NodeMap Parse( ticpp::Node* node )
   {
      Insert( node, "module_name" );
      Insert( node, "input_width" );
      Insert( node, "output_width" );
      Insert( node, "min_decimation" );
      Insert( node, "max_decimation" );
      Insert( node, "differential_delay");
      Insert( node, "num_stages" );

      return map_;
   }

   const std::string& Name() { return name_; }

};

#endif
