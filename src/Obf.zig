const std = @import("std");
const package = @import("package.zig");

const this = @This();

pub const ObfError = error{
    reader_null,
    writer_null,
};

pub const Config = struct {
    reader: ?std.io.AnyReader = null,
    writer: ?std.io.AnyWriter = null,
};

const Mode = enum(i8) {
    package_mode = 0x90,
    file_mode = 0x91,
};

allocator: std.mem.Allocator,
reader: ?std.io.AnyReader,
writer: ?std.io.AnyWriter,

/// init this struct (Obf)
pub fn init(config: Config) !this {
    const gpa = std.heap.GeneralPurposeAllocator(.{}){};
    return .{
        .allcoator = gpa.allocator(),
        .reader = config.reader,
        .writer = config.writer,
    };
}

pub const ReadPackage = struct {
    const this = @This();
    arena: std.heap.ArenaAllocator,
    packages: []package.Package,

    pub fn destroy(self: *this) void {
        arena.deinit();
    }
};

pub fn read(self: *this) !ReadPackage {
    if (self.reader == null) {
        return ObfError.reader_null;
    }
    var read_packages: ReadPackage = .{.arena = std.heap.ArenaAllocator.init(self.allocator)};

    const reader = self.reader.?;
    const length_of_elems = try reader.readInt(usize, .little); 
    const allocator = read_packages.arena.allocator();

    read_packages.packages = allocator.alloc(package.Package, length_of_elems);
    for(read_packages.packages) |pkg| {
        
        const length_of_package_string = try reader.readInt(usize, .little);
        pkg.name = allocator.alloc(u8, length_of_package_string);
        _ = try reader.read(pkg.name);

        const length_of_version_string = try reader.readInt(usize, .little);
        pkg.version = allocator.alloc(u8, length_of_version_string);
        _ = try reader.read(pkg.version);

        pkg.generation = try reader.readInt(usize, .little);
        
        const files_on = try reader.readInt(u1, .little);
        const licenses = try reader.readInt(u1, .little);
        const authors_on = try reader.readInt(u1, .little);
        const maintainers_on = try reader.readInt(u1, .little);
        const upstream_urls_on = try reader.readInt(u1, .little);

        const dependencies_on = try reader.readInt(u1, .little);
        const suggested_on = try reader.readInt(u1, .little);
        const optional_on = try reader.readInt(u1, .little);
        const build_on = try reader.readInt(u1, .little);
        const conflicts_on = try reader.readInt(u1, .little);
    }
}
