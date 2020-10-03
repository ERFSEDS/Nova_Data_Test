
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

			("s,step_size", "How large are the time steps between reported values. Lover values yield higher precision but may take more time", cxxopts::value<double>()->default_value("0.0001"))
			("oversampling_rate", "Determines how many sub-updates are applied between reported values", cxxopts::value<int>()->default_value("10"))

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
				params.OversamplingRate = result["oversampling_rate"].as<int>();
				if (params.OversamplingRate <= 0)
				{
					params.OversamplingRate = 1;
				}
				const auto& outputPath = result["out_file"].as<std::string>();
				params.OutputFile.open(outputPath, std::ios::out);
				if (!params.OutputFile.good())
				{
					NOVA_ERROR("Failed to open output file \"{}\"", outputPath);
					return EXIT_FAILURE;
				}
				NOVA_TRACE("Successfully opened output file \"{}\"", outputPath);

				const auto error = Parser::ParseShipJSON(inputJSON, params.Ships);
				if (error.length())
				{
					throw std::logic_error("Failed to parse rocket JSON! Error: " + error);
				}

				Simulation simulation(params);

				simulation.Run();

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
