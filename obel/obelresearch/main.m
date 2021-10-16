//
//  main.m
//  obelresearch
//
//  Created by Jack Mousseau on 9/24/21.
//

#import <obel/obel.h>

int main(int argc, const char * argv[]) {
    const char *xyz_path = "/Users/jack/Desktop/obel/bunny.xyz";
    const char *ply_path = "/Users/jack/Desktop/obel/bunny_simplified.ply";

    int xyz_fd, ply_fd;
    if ((xyz_fd = open(xyz_path, O_RDWR)) < 0) { exit(1); }
    if ((ply_fd = open(ply_path, O_RDWR | O_CREAT)) < 0) { exit(1); }

    obel_cloud_t cloud = { 0 };
    obel_cloud_read_xyz(xyz_fd, &cloud);

    obel_cloud_t simplified = { .capacity = cloud.capacity / 2 };
    obel_cloud_map(&simplified);

    obel_arena_t *arena = obel_arena_alloc(OBEL_MEBIBYTES(24));

    obel_cloud_simplify(arena, &cloud, &simplified);
    obel_arena_free(arena);
    obel_cloud_unmap(&cloud);
    close(xyz_fd);

    obel_cloud_write_ply(ply_fd, &simplified);
    obel_cloud_unmap(&simplified);
    close(ply_fd);

    return 0;
}
