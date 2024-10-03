#pragma once

#include "Boksi/Core/Resource/Buffer.h"

#include <filesystem>
#include <cstdint>

namespace Boksi
{
	class FileIO
	{
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
		static Buffer ReadFileBinary(const std::filesystem::path& filepath, uint64_t offset, uint64_t size);
		static std::string ReadFileText(const std::filesystem::path& filepath);

		static bool WriteFileBinary(const std::filesystem::path& filepath, const Buffer& buffer);
		static bool WriteFileText(const std::filesystem::path& filepath, const std::string& text);
	};
}