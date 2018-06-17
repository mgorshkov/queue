#define BOOST_TEST_MODULE queue_server_test

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

/**
 * @brief Unit tests for queue project.
 */
BOOST_AUTO_TEST_SUITE(queue_server_test)

BOOST_AUTO_TEST_CASE(test_mmap_create)
{
    using MappedFile = boost::iostreams::mapped_file;

    const char* fileName = "test.data";
    boost::iostreams::mapped_file_params mappedFileParams;
    mappedFileParams.path          = fileName;
    mappedFileParams.new_file_size = 0x1000;
    mappedFileParams.flags         = boost::iostreams::mapped_file::mapmode::readwrite;

    MappedFile mappedFile(mappedFileParams);
    char* ptr = mappedFile.data();
    strcpy(ptr, "123123123123123123");

    const char* constPtr = mappedFile.const_data();
    BOOST_CHECK(std::string(ptr) == std::string(constPtr));

    boost::filesystem::remove(fileName);
}

}
