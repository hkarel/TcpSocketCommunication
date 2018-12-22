import qbs
import QbsUtl
import ProbExt

Product {
    name: "Server2"
    targetName: "server2"
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
        "../communication/commands.cpp",
        "../communication/commands.h",
        "server2.cpp",
        "server2_appl.cpp",
        "server2_appl.h",
    ]

//    property var test: {
//        console.info("=== cpp.staticLibraries ===");
//        console.info(cpp.staticLibraries);
//        console.info("=== qbs.architecture ===");
//        console.info(qbs.architecture);
//        console.info("=== cpp.compilerLibraryPaths ===");
//        console.info(cpp.compilerLibraryPaths);
//        console.info("=== cpp.compilerPath ===");
//        console.info(cpp.compilerPath);
//        console.info("=== targetOS ===");
//        console.info(qbs.targetOS);
//        console.info("=== buildDirectory ===");
//        console.info(project.buildDirectory);
//    }

} // Product
