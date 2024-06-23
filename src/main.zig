const std = @import("std");
const ansi = @import("ansi");
const lua = @import("lua");
const builtin = @import("builtin");
const fs = @import("fs.zig");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    const argv = try std.process.argsAlloc(allocator);
    defer std.process.argsFree(allocator, argv);

    const stdout = std.io.getStdOut();
    defer stdout.close();

    const program = argv[0];
    const args = argv[1..];

    for (args, 0..) |arg, i| {
        _ = i;
        if (std.mem.eql(u8, "-h", arg) or std.mem.eql(u8, "--help", arg)) {
            try stdout.writer().print(
                \\{s}:
                \\    -h, --help                      this message
                \\    -v, --version                   the version
                \\    -i, --install <PKGS>            installs packages from repos
                \\    -r, --remove  <PKGS>            remove installed packages
                \\
                \\    -s, --sync [REPOS]              same as -u
                \\    -u, --update  [REPOS]           update all repos unless specified  
                \\    -U, --upgrade                   upgrade the system
                \\    -q, --query <CRITERIA>          queries t             
                \\    -L, --list                      list packages installed on the system
                \\    -b, --build [BUILDFILE PATH]    can be used in combination with -l to automatically install the built package,
                \\                                    builds a package (by default ./BUILDFILE.lua / ./buildfile.lua)
                \\    -l, --locinstall [FILE]         can be used in combination with -b to automatically install the built package,
                \\                                    installs a local package
                \\    -g, --generate-index            generates an Obf index file for repository, requires a REPO.toml/repo.toml
            , .{program});
        }
    }
}
