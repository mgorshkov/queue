#include <future>
#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

/**
 * @brief Unit tests for queue project.
 */
BOOST_AUTO_TEST_SUITE(queue_map_test)

BOOST_AUTO_TEST_CASE(test_mmap_create)
{
    using MappedFile = boost::iostreams::mapped_file;

    const char* fileName = "test.data";
    boost::iostreams::mapped_file_params mappedFileParams;
    mappedFileParams.path          = fileName;
    mappedFileParams.new_file_size = 0x1000;
    mappedFileParams.flags         = boost::iostreams::mapped_file::mapmode::readwrite;

    {
        MappedFile mappedFile(mappedFileParams);
        char* ptr = mappedFile.data();
        strcpy(ptr, "123123123123123123");

        const char* constPtr = mappedFile.const_data();
        BOOST_CHECK(std::string(ptr) == std::string(constPtr));
    }

    boost::filesystem::remove(fileName);
}

BOOST_AUTO_TEST_CASE(test_mmap_write_read)
{
    using MappedFile = boost::iostreams::mapped_file;

    const char* fileName = "test.data";
    boost::iostreams::mapped_file_params mappedFileParams;
    mappedFileParams.path          = fileName;
    mappedFileParams.flags         = boost::iostreams::mapped_file::mapmode::readwrite;

    {
        mappedFileParams.new_file_size = 0x1000;

        MappedFile mappedFile(mappedFileParams);
        char* ptr = mappedFile.data();
        strcpy(ptr, "123123123123123123");
    }

    {
        mappedFileParams.new_file_size = 0;

        MappedFile mappedFile(mappedFileParams);
        const char* constPtr = mappedFile.const_data();
        BOOST_CHECK(std::string(constPtr) == "123123123123123123");
    }
    boost::filesystem::remove(fileName);
}

}
