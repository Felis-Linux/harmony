const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "harmony",
        .root_source_file = b.path("src/main.zig"),
        .target = target,
        .optimize = optimize,
    });
    const ziglua = b.dependency("ziglua", .{ .target = target, .optimize = optimize });
    const ansi = b.dependency("ansi-term", .{ .target = target, .optimize = optimize });
    exe.root_module.addImport("lua", ziglua.module("ziglua"));
    exe.root_module.addImport("ansi", ansi.module("ansi-term"));

    b.installArtifact(exe);
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);

    const main_tests = b.addTest(.{
        .root_source_file = b.path("src/main.zig"),
        .target = target,
        .optimize = optimize,
    });

    const fs_tests = b.addTest(.{
        .root_source_file = b.path("src/fs.zig"),
        .target = target,
        .optimize = optimize,
    });

    fs_tests.root_module.addImport("ansi", ansi.module("ansi-term"));
    const run_main_tests = b.addRunArtifact(main_tests);
    const run_fs_tests = b.addRunArtifact(fs_tests);

    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&run_main_tests.step);
    test_step.dependOn(&run_fs_tests.step);
}
