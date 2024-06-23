const std = @import("std");

pub const Repository = struct {
    const this = @This();

    name: []const u8,
    url: []const u8,
    packages: std.ArrayList(Package),

    arena: std.heap.ArenaAllocator,

    pub fn destory(self: *this) void {
        self.arena.deinit();
    }
};

pub const Package = struct {
    name: []const u8,
    version: []const u8,
    generation: usize,

    metadata: PackageMetadata,
};

pub const PackageMetadata = struct {
    files: ?[][]const u8 = null,
    licenses: ?[][]const u8 = null,
    authors: ?[][]const u8 = null,
    maintainers: ?[][]const u8 = null,
    upstream_urls: ?[][]const u8 = null,

    dependencies: ?[][]const u8 = null,
    suggested: ?[][]const u8 = null,
    optional: ?[][]const u8 = null,
    build: ?[][]const u8 = null,
    conflicts: ?[][]const u8 = null,
};

pub fn parseCriteria(criteria: []const u8) !?Package {
    std.mem.split(u8, criteria, "= ");
}
