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

#include <sdr_simulator/xml/DownConverterXmlParser.hpp>
#include <sdr_simulator/util/CodeGenerator.hpp>

#include "StimulusXmlParser.hpp"

using namespace std;
using namespace code_generator;
using namespace boost;

int main(void)
{
   const std::string CONFIG_FILE_NAME = "../../../config/sdr_config.xml";
   const std::string STIMULUS_FILE_NAME = "stimulus.xml";
   const string HEADER_FILE_NAME = "test_bench.hpp";

   // open the configuration file for parsing
   ticpp::Document doc( CONFIG_FILE_NAME );

   // Create a parser object
   DownConverterXmlParser ddc_parser;

   // Parse the xml file.
   doc.LoadFile();

   // Use the root node for reference.
   ticpp::Node* root = doc.FirstChild();

   // find the first module node in the xml file
   ticpp::Node* node = root->FirstChildElement( ddc_parser.Name() );

   // Retrieve a map containing accumulator keywords
   xml::NodeMap config_map = ddc_parser.Parse( node ); 

   // Create a parser object
   StimulusXmlParser stimulus_parser;

   doc.LoadFile( STIMULUS_FILE_NAME );
   root = doc.FirstChildElement( stimulus_parser.Name() );
   xml::NodeMap stimulus_map = stimulus_parser.Parse( root );

   const int INPUT_WIDTH = 
      lexical_cast<int>( stimulus_map["input_width"] );
   const double OUTPUT_WIDTH = 
      lexical_cast<int>( stimulus_map["output_width"] );
   const int COEFF_WIDTH = 
      lexical_cast<int>( stimulus_map["coeff_width"] );
   const int ACCUMULATOR_WIDTH = 
      lexical_cast<int>( stimulus_map["accumulator_width"] );
   const double MEAN = 
      lexical_cast<double>( stimulus_map["mean"] );
   const double VARIANCE = 
      lexical_cast<double>( stimulus_map["variance"] );
   const double AMPLITUDE = 
      lexical_cast<double>( stimulus_map["amplitude"] );

   // create a CodeGenerator object. This is required to generate the
   // header file. 
   code_generator::CodeGenerator code_generator;

   // generate data input type
   code_generator.AddTypeDef(
         "data_input_type",
         "sc_int<" + lexical_cast< string >( INPUT_WIDTH ) + ">"
         );

   // generate data output type
   code_generator.AddTypeDef(
         "data_output_type",
         "sc_int<" + lexical_cast< string >( OUTPUT_WIDTH ) + ">"
         );

   code_generator.AddTypeDef(
         "reset_type",
         "bool"
         );

   code_generator.AddConstant<int>(
         "ACCUMULATOR_WIDTH",
         ACCUMULATOR_WIDTH
         );

   code_generator.AddConstant<int>(
         "COEFF_WIDTH",
         COEFF_WIDTH
         );

   code_generator.AddConstant<int>(
         "INPUT_WIDTH",
         INPUT_WIDTH
         );

   code_generator.AddConstant<int>(
         "OUTPUT_WIDTH",
         OUTPUT_WIDTH
         );

   code_generator.AddConstant<double>(
         "MEAN",
         MEAN
         );
   
   code_generator.AddConstant<double>(
         "VARIANCE",
         VARIANCE
         );

   code_generator.AddConstant<double>(
         "AMPLITUDE",
         AMPLITUDE
         );

   //TODO: Remove and place in stimulus config

   code_generator.AddInclude( "systemc.h", true );

   code_generator.GenerateFile( HEADER_FILE_NAME );

   return EXIT_SUCCESS;
}