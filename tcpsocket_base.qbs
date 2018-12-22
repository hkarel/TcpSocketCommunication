import qbs
import qbs.File
import "qbs/imports/QbsUtl/qbsutl.js" as QbsUtl

Project {
    minimumQbsVersion: "1.12.0"
    qbsSearchPaths: ["qbs"]

    property var cppDefines: {
        var def = [
            "QDATASTREAM_VERSION=QDataStream::Qt_4_8",
            "BPROTOCOL_VERSION_LOW=0",
            "BPROTOCOL_VERSION_HIGH=0",
        ];

        if (qbs.buildVariant === "release")
            def.push("NDEBUG");

        return def;
    }

    property var cxxFlags: [
        //"-std=c++14",
        //"-msse4",
        "-ggdb3",
        "-Winline",
        "-Wall",
        "-Wextra",
        "-Wno-unused-parameter",
        "-Wno-variadic-macros",
    ]
    property string cxxLanguageVersion: "c++14"
}
