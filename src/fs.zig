const std = @import("std");
const ansi = @import("ansi");

/// Options for Downloader.downloadOne
pub const DownloadOneOption = struct {
    offset: usize = 0,
    check_checksum: bool = false,
};

/// Downloader context
pub const Downloader = struct {
    pub const DownloaderError = error{
        download_size_is_zero,
    };
    const this = @This();
    const header_size = 128_000;
    const buffer_size = 1024_000;

    allocator: std.mem.Allocator,
    stdout: std.fs.File,
    stderr: std.fs.File,

    /// intializes the whole Downloader, including
    /// the allocator context
    pub fn init() this {
        var gpa = std.heap.GeneralPurposeAllocator(.{}){};
        return .{
            .allocator = gpa.allocator(),
            .stdout = std.io.getStdOut(),
            .stderr = std.io.getStdErr(),
        };
    }

    /// internal progress function for the Downloader
    fn progress(self: *this, filename: []const u8, downloaded: usize, to_download: usize, offset: usize) !void {
        try ansi.cursor.setCursor(self.stdout.writer(), 0, offset);
        const downloaded_f: f64 = @floatFromInt(downloaded);
        const to_download_f: f64 = @floatFromInt(to_download);
        try self.stdout.writer().print("[{}%] {s}", .{ @trunc(downloaded_f / to_download_f * 100), filename });
    }

    /// downloads a single file and pushes it into a writer.
    /// caller is responsible for the File context.
    pub fn downloadOne(self: *this, url: []const u8, writer: std.io.AnyWriter, opt: DownloadOneOption) !void {
        const uri: std.Uri = try std.Uri.parse(url);
        var client = std.http.Client{ .allocator = self.allocator };
        defer client.deinit();

        const header_buffer: []u8 = try self.allocator.alloc(u8, header_size);
        defer self.allocator.free(header_buffer);

        var reqst: std.http.Client.Request = try client.open(.GET, uri, .{ .server_header_buffer = header_buffer });
        defer reqst.deinit();

        try reqst.wait();
        const data_buffer: []u8 = try self.allocator.alloc(u8, buffer_size);
        defer self.allocator.free(data_buffer);

        var downloaded: usize = 0;
        const to_download: usize = reqst.response.content_length.?;
        if (to_download == 0) {
            return DownloaderError.download_size_is_zero;
        }

        while (true) {
            const d = try reqst.read(data_buffer);
            if (d == 0) break;
            downloaded += d;
            _ = try writer.write(data_buffer);
            try self.progress(std.fs.path.basename(uri.path.raw), downloaded, to_download, opt.offset);
        }
    }

    pub fn destroy(self: *this) void {
        self.stdout.close();
        self.stderr.close();
    }
};

test "downloader" {
    var allocator = std.testing.allocator;
    const writer_buffer: []u8 = try allocator.alloc(u8, 2048_000);
    defer allocator.free(writer_buffer);
    var stack_writer = std.io.fixedBufferStream(writer_buffer);

    var downloader: Downloader = Downloader.init();
    defer downloader.destroy();
    try downloader.downloadOne("https://pl.wikipedia.org/wiki/Plik:Wikipedia-logo-v2.svg", stack_writer.writer().any(), .{});

    const original = @embedFile("tests/Wikipedia-logo-v2.svg");

    var checksum_a: [32]u8 = undefined;

    var checksum_b: [32]u8 = undefined;

    std.crypto.hash.sha2.Sha256.hash(writer_buffer, &checksum_a, .{});
    std.crypto.hash.sha2.Sha256.hash(original, &checksum_b, .{});

    try std.testing.expectEqualStrings(writer_buffer, original);
}
