import qbs
import QbsUtl
import ProbExt

Product {
    name: "Client2"
    targetName: "client2"
    condition: true

    type: "application"
    destinationDirectory: "./bin"

    Depends { name: "cpp" }
    Depends { name: "cppstdlib" }
    Depends { name: "SharedLib" }
    Depends { name: "RapidJson" }
    Depends { name: "Yaml" }
    Depends { name: "Qt"; submodules: ["core", "network"] }

    cpp.defines: {
        var def = project.cppDefines;
        return def;
    }

    cpp.cxxFlags: project.cxxFlags
    cpp.cxxLanguageVersion: project.cxxLanguageVersion

    cpp.includePaths: [
        "./",
        "../",
    ]
    cpp.systemIncludePaths: QbsUtl.concatPaths(
        Qt.core.cpp.includePaths
    )

    cpp.rpaths: QbsUtl.concatPaths(
        cppstdlib.path,
        "$ORIGIN/../lib"
    )

    cpp.dynamicLibraries: {
        var libs = [
            "pthread",
        ];
        return libs;
    }

    files: [
        "client2.cpp",
        "client2_appl.cpp",
        "client2_appl.h",
        "../communication/commands.cpp",
        "../communication/commands.h",
    ]

} // Product
