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
            stdout.writer().print(
                \\{s}:
                \\    -h, --help                      this message
                \\    -v, --version                   the version
                \\    -i, --install <PKGS>            installs packages from repos
                \\    -l, --locinstall <FILE>         installs a tar.zst archive
                \\    -r, --remove  <PKGS>            remove installed packages
                \\
                \\    -s, --sync [REPOS]
                \\    -u, --update  [REPOS]           update all repos unless specified  
                \\    -U, --upgrade                   upgrade the system
                \\    -q, --query                     takes subflags -p and -f
                \\        -p, --package               package mode
                \\        -f, --file                  file mode
                \\    -l, --list                      list packages installed on the system
            , .{program});
        }
    }
}
