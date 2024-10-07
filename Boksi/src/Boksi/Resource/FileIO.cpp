#include "bkpch.h"

#include "FileIO.h"

namespace Boksi
{
	Buffer FileIO::ReadFileBinary(const std::filesystem::path& filepath)
	{
		BK_PROFILE_FUNCTION();

		if ( !std::filesystem::exists(filepath) )
		{
			BK_CORE_ERROR("FileIO::ReadFileBinary: File does not exist: {0}", filepath.string());
			return {};
		}

		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);
		if ( !stream )
		{
			BK_CORE_ERROR("FileIO::ReadFileBinary: Failed to open file: {0}", filepath.string());
			return {};
		}

		const std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		const uint64_t size = static_cast<uint64_t>(end - stream.tellg());

		if (size == 0)
		{
			// File is empty
			BK_CORE_WARN("FileIO::ReadFileBinary: File is empty: {0}", filepath.string());
			return {};
		}

		Buffer buffer(size);
		stream.read(buffer.As<char>(), static_cast<int64_t>(size));
		stream.close();

		return buffer;
	}

	Buffer FileIO::ReadFileBinary(const std::filesystem::path& filepath, uint64_t offset, uint64_t size)
	{
		BK_PROFILE_FUNCTION();

		throw std::logic_error("Not Implemented!");
		return {};
	}

	std::string FileIO::ReadFileText(const std::filesystem::path& filepath)
	{
		BK_PROFILE_FUNCTION();

		if (!std::filesystem::exists(filepath))
		{
			BK_CORE_ERROR("FileIO::ReadFileBinary: File does not exist: {0}", filepath.string());
			return {};
		}

		std::ifstream stream(filepath, std::ios::in | std::ios::binary);
		if (!stream)
		{
			BK_CORE_ERROR("FileIO::ReadFileBinary: Failed to open file: {0}", filepath.string());
			return {};
		}

		stream.seekg(0, std::ios::end);
		const uint64_t size = stream.tellg();
		if (size == 0)
		{
			// File is empty
			BK_CORE_WARN("FileIO::ReadFileBinary: File is empty: {0}", filepath.string());
			return {};
		}

		std::string result;
		result.resize(size);
		stream.seekg(0, std::ios::beg);
		stream.read(result.data(), static_cast<int64_t>(size));

		return result;
	}

	bool FileIO::WriteFileBinary(const std::filesystem::path& filepath, const Buffer& buffer)
	{
		BK_PROFILE_FUNCTION();

		throw std::logic_error("Not Implemented!");
		return false;
	}

	bool FileIO::WriteFileText(const std::filesystem::path& filepath, const std::string& text)
	{
		BK_PROFILE_FUNCTION();

		throw std::logic_error("Not Implemented!");
		return false;
	}
}