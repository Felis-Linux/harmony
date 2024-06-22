const std = @import("std");

pub const Package = struct {
    name: []const u8,
    version: []const u8,
    generation: usize,

    metadata: PackageMetadata,
};

pub const PackageMetadata = struct {
    files: ?[][]const u8 = null,
    licenses: ?[][]const u8 = null,
    authors: ?[][]const u8,
    maintainers: ?[][]const u8,
    upstream_urls: ?[][]const u8,

    dependencies: ?[][]const u8,
    suggested: ?[][]const u8,
    optional: ?[][]const u8,
    build: ?[][]const u8,
    conflicts: ?[][]const u8,
};
