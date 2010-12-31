// Copyright (c) 2010 Ryan Seal <rlseal -at- gmail.com>
//
// This file is part of Software Defined Radar Simulator (SDRS) Software.
//
// SDRS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//  
// SDRS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SDRS.  If not, see <http://www.gnu.org/licenses/>.
#include <iostream>
#include <tr1/math.h>

#include <sdr_simulator/filter/cic/CicGenerator.hpp>
#include <sdr_simulator/xml/CicXmlParser.hpp>
#include <sdr_simulator/util/CodeGenerator.hpp>

#include "StimulusXmlParser.hpp"

using namespace std;
using namespace code_generator;
using namespace boost;

int main(void)
{
   const std::string CONFIGURATION_FILE_NAME = "../../config/sdr_config.xml";
   const std::string STIMULUS_FILE_NAME = "stimulus.xml";
   const string HEADER_FILE_NAME = "test_bench.hpp";

   // open the configuration file for parsing
   ticpp::Document doc( CONFIGURATION_FILE_NAME );

   // Create a parser object
   CicXmlParser cic_parser;

   // Parse the xml file.
   doc.LoadFile();

   // Use the root node for reference.
   ticpp::Node* root = doc.FirstChild();

   // find the first module node in the xml file
   ticpp::Node* node = root->FirstChildElement( cic_parser.Name() );

   // Retrieve a map containing accumulator keywords
   xml::NodeMap config_map = cic_parser.Parse( node ); 

   // create constants that were read in from the xml file.
   const int INPUT_WIDTH = lexical_cast<int>( config_map["input_width"] );
   const int OUTPUT_WIDTH = lexical_cast<int>( config_map["output_width"] );

   doc.LoadFile( STIMULUS_FILE_NAME );

   StimulusXmlParser stimulus_parser;

   root = doc.FirstChild( stimulus_parser.Name() );

   // Retrieve a map containing accumulator keywords
   xml::NodeMap stimulus_map = stimulus_parser.Parse( root ); 

   // create constants that were read in from the xml file.
   const int COEFF_WIDTH = lexical_cast<int>( stimulus_map["coeff_width"] );
   const int SUM_WIDTH = lexical_cast<int>( stimulus_map["sum_width"] );
   const int DECIMATION_WIDTH = lexical_cast<int>( stimulus_map["decimation_width"] );
   const int DECIMATION = lexical_cast<int>( stimulus_map["decimation"] );

   // create a CodeGenerator object. This is required to generate the
   // header file. 
   code_generator::CodeGenerator code_generator;

   
   // generate data input type
   code_generator.AddTypeDef(
         "INPUT_DATA_TYPE",
         "sc_int<" + lexical_cast< string >( INPUT_WIDTH ) + ">"
         );

   // generate data output type
   code_generator.AddTypeDef(
         "OUTPUT_DATA_TYPE",
         "sc_int<" + lexical_cast< string >( OUTPUT_WIDTH ) + ">"
         );

   // generate decimation type
   code_generator.AddTypeDef(
         "DECIMATION_TYPE",
         "sc_int<" + lexical_cast< string >( DECIMATION_WIDTH ) + ">"
         );

   code_generator.AddTypeDef(
         "RESET_TYPE",
         "bool"
         );

   code_generator.AddConstant<int>(
         "COEFF_WIDTH",
         COEFF_WIDTH
         );

   code_generator.AddConstant<int>(
         "SUM_WIDTH",
         SUM_WIDTH
         );

   code_generator.AddConstant<int>(
         "DECIMATION",
         DECIMATION
         );

   code_generator.AddInclude( "systemc.h", true );

   code_generator.GenerateFile( HEADER_FILE_NAME );

   CicGenerator cicGenerator( CONFIGURATION_FILE_NAME );

   return EXIT_SUCCESS;
}
