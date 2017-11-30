#include "utils.h"
#include "gtest/gtest.h"

TEST(Test_UtilsFileIO, GetSuffix) {
    // windows path style, mixed style
    EXPECT_EQ("tif", utilsFileIO::GetSuffix("c:/test/dem.tif"));
    EXPECT_EQ("fig", utilsFileIO::GetSuffix("c:\\test\\config.fig"));
    EXPECT_EQ("txt", utilsFileIO::GetSuffix("c:\\test/file.txt"));
    // linux or unix sytle
    EXPECT_EQ("tif", utilsFileIO::GetSuffix("/usr/seims/data/dem.tif"));
    // without suffix
    EXPECT_EQ("", utilsFileIO::GetSuffix("abcdefg"));
}