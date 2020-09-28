
#include <iostream>
#include <fstream>
#include <filesystem>
#include <exception>

#include <cxxopts.hpp>

#include "Simulation.h"
#include "Parser.h"
#include "Log.h"

using namespace erfseds_nova::test_data;

int main(int argc, const char** argv)
{
	try
	{
		cxxopts::Options options("test_data", "A C++ based CLI program that generates simulated rocket data to compare with real sensors");

		options.add_options()
			("o,out_file", "Where the output data is written (in csv format)", cxxopts::value<std::string>()->default_value("TestData.csv"))
			("i,input_file", "A json file containing an array of rocket objects to simulate", cxxopts::value<std::string>()->default_value("Rockets.json"))

			("s,step_size", "How large integration step sizes are in seconds. Lover values yield higher precision but may take more time", cxxopts::value<double>()->default_value("0.0001"))
			("d,debug", "Enable debug logging", cxxopts::value<bool>()->default_value("false"))
			("h,help", "Print this help menu")
		;
		auto result = options.parse(argc, argv);

		try
		{
			bool debug = result["debug"].as<bool>();
			Log::Init(debug);


			if (result.count("help"))
			{
				std::cout << options.help() << std::endl;
			}
			else
			{
				SimulationParameters params;

				std::filesystem::path inputJSON = result["input_file"].as<std::string>();
				if (!std::filesystem::exists(inputJSON))
				{
					NOVA_ERROR("Failed to find input file {}", inputJSON);
					return EXIT_FAILURE;
				}

				params.StepSize = result["step_size"].as<double>();
				params.OutputFile = result["out_file"].as<std::string>();

				const auto error = Parser::ParseBodyJSON(inputJSON, params.Bodies);
				if (error.length())
				{
					throw std::logic_error("Failed to parse rocket JSON! Error: " + error);
				}

				Simulation simulation(params);

			}
		} catch (const std::exception& e) {
			NOVA_ERROR("{}", e.what());
			return EXIT_FAILURE;
		}
	
		return EXIT_SUCCESS;
	} catch (const std::exception& e) {
		std::cout << "Error parsing arguments: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

}
