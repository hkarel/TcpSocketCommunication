import qbs
import qbs.File
import "qbs/imports/QbsUtl/qbsutl.js" as QbsUtl

Project {
    minimumQbsVersion: "1.12.0"
    qbsSearchPaths: ["qbs"]

    property var cppDefines: {
        var def = [
            "__cplusplus=201103L",
            "BPROTOCOL_VERSION_LOW=0",
            "BPROTOCOL_VERSION_HIGH=0",
        ];

        if (qbs.buildVariant === "release")
            def.push("NDEBUG");

        return def;
    }

    property var cxxFlags: [
        "-std=c++11",
        "-ggdb3",
        "-Winline",
        "-Wall",
        "-Wextra",
        "-Wno-unused-parameter",
        "-Wno-variadic-macros",
    ]
}
