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
    arena: std.heap.ArenaAllocator,
    packages: []package.Package,

    pub fn destroy(self: *ReadPackage) void {
        self.arena.deinit();
    }
};

pub fn read(self: *this) !ReadPackage {
    if (self.reader == null) {
        return ObfError.reader_null;
    }
    var read_packages: ReadPackage = .{ .arena = std.heap.ArenaAllocator.init(self.allocator) };

    const reader = self.reader.?;
    const length_of_elems = try reader.readInt(usize, .little);
    const allocator = read_packages.arena.allocator();

    read_packages.packages = allocator.alloc(package.Package, length_of_elems);
    for (read_packages.packages) |pkg| {
        const length_of_package_string = try reader.readInt(usize, .little);
        pkg.name = allocator.alloc(u8, length_of_package_string);
        _ = try reader.read(pkg.name);

        const length_of_version_string = try reader.readInt(usize, .little);
        pkg.version = allocator.alloc(u8, length_of_version_string);
        _ = try reader.read(pkg.version);

        pkg.generation = try reader.readInt(usize, .little);

        const files_on = try reader.readInt(u1, .little);
        const licenses_on = try reader.readInt(u1, .little);
        const authors_on = try reader.readInt(u1, .little);
        const maintainers_on = try reader.readInt(u1, .little);
        const upstream_urls_on = try reader.readInt(u1, .little);

        const dependencies_on = try reader.readInt(u1, .little);
        const suggesteds_on = try reader.readInt(u1, .little);
        const optionals_on = try reader.readInt(u1, .little);
        const builds_on = try reader.readInt(u1, .little);
        const conflicts_on = try reader.readInt(u1, .little);

        if (files_on == 1) {
            const files_len = try reader.readInt(usize, .little);
            pkg.metadata.files = self.allocator.alloc(u8, files_len);
            for (pkg.metadata.files) |file| {
                const file_len = try reader.readInt(usize, .little);
                file = self.allocator.alloc(u8, file_len);
                _ = try reader.read(file);
            }
        }

        if (licenses_on == 1) {
            const licenses_len = try reader.readInt(usize, .little);
            pkg.metadata.licenses = self.allocator.alloc(u8, licenses_len);
            for (pkg.metadata.licenses) |license| {
                const license_len = try reader.readInt(usize, .little);
                license = self.allocator.alloc(u8, license_len);
                _ = try reader.read(license);
            }
        }

        if (authors_on == 1) {
            const authors_len = try reader.readInt(usize, .little);
            pkg.metadata.authors = self.allocator.alloc(u8, authors_len);
            for (pkg.metadata.authors) |author| {
                const author_len = try reader.readInt(usize, .little);
                author = self.allocator.alloc(u8, author_len);
                _ = try reader.read(author);
            }
        }

        if (maintainers_on == 1) {
            const maintainers_len = try reader.readInt(usize, .little);
            pkg.metadata.maintainers = self.allocator.alloc(u8, maintainers_len);
            for (pkg.metadata.maintainers) |maintainer| {
                const maintainer_len = try reader.readInt(usize, .little);
                maintainer = self.allocator.alloc(u8, maintainer_len);
                _ = try reader.read(maintainer);
            }
        }

        if (upstream_urls_on == 1) {
            const upstream_urls_len = try reader.readInt(usize, .little);
            pkg.metadata.upstream_urls = self.allocator.alloc(u8, upstream_urls_len);
            for (pkg.metadata.upstream_urls) |upstream_url| {
                const upstream_url_len = try reader.readInt(usize, .little);
                upstream_url = self.allocator.alloc(u8, upstream_url_len);
                _ = try reader.read(upstream_url);
            }
        }

        if (dependencies_on == 1) {
            const dependencies_len = try reader.readInt(usize, .little);
            pkg.metadata.dependencies = self.allocator.alloc(u8, dependencies_len);
            for (pkg.metadata.dependencies) |dependency| {
                const dependency_len = try reader.readInt(usize, .little);
                dependency = self.allocator.alloc(u8, dependency_len);
                _ = try reader.read(dependency);
            }
        }

        if (suggesteds_on == 1) {
            const suggesteds_len = try reader.readInt(usize, .little);
            pkg.metadata.suggesteds = self.allocator.alloc(u8, suggesteds_len);
            for (pkg.metadata.suggesteds) |suggested| {
                const suggested_len = try reader.readInt(usize, .little);
                suggested = self.allocator.alloc(u8, suggested_len);
                _ = try reader.read(suggested);
            }
        }

        if (optionals_on == 1) {
            const optionals_len = try reader.readInt(usize, .little);
            pkg.metadata.optionals = self.allocator.alloc(u8, optionals_len);
            for (pkg.metadata.optionals) |optional| {
                const optional_len = try reader.readInt(usize, .little);
                optional = self.allocator.alloc(u8, optional_len);
                _ = try reader.read(optional);
            }
        }

        if (builds_on == 1) {
            const builds_len = try reader.readInt(usize, .little);
            pkg.metadata.builds = self.allocator.alloc(u8, builds_len);
            for (pkg.metadata.builds) |build| {
                const build_len = try reader.readInt(usize, .little);
                build = self.allocator.alloc(u8, build_len);
                _ = try reader.read(build);
            }
        }

        if (conflicts_on == 1) {
            const conflicts_len = try reader.readInt(usize, .little);
            pkg.metadata.conflicts = self.allocator.alloc(u8, conflicts_len);
            for (pkg.metadata.conflicts) |conflict| {
                const conflict_len = try reader.readInt(usize, .little);
                conflict = self.allocator.alloc(u8, conflict_len);
                _ = try reader.read(conflict);
            }
        }
    }
}
