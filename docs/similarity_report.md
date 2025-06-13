# QPsdView Similarity Test Results

Generated on: 2025-06-13T10:08:00

## How to Update This Report

To regenerate this report and update the comparison images, run the following command from the project root:

```bash
QTPSD_SIMILARITY_SUMMARY_PATH=docs ctest -R tst_qpsdview
```

Or directly:

```bash
QTPSD_SIMILARITY_SUMMARY_PATH=docs ./<BUILD_DIR>/tests/auto/psdwidget/tst_qpsdview
```

Where `<BUILD_DIR>` is your build directory (e.g., `build`, `build/SDK_Qt_6-Debug`, `build-debug`, etc.)

This will:
- Run the QPsdView comparison tests
- Save comparison images to `docs/images/`
- Generate this report as `docs/similarity_report.md`

**Note**: The environment variable `QTPSD_SIMILARITY_SUMMARY_PATH` specifies where to save the report. If not set, no report is generated. The path can be absolute or relative to the project root.

## Summary Statistics

| Metric | Value |
|--------|-------|
| Total Tests | 31 |
| Passed Tests | 30 (96.8%) |
| Failed Tests | 1 |
| Average Similarity | 89.45% |
| Minimum Similarity | 25.00% |
| Maximum Similarity | 100.00% |

## Test Results

| File | Similarity | Status | PSD | QtPsd | Difference |
|------|------------|--------|-----|-------|------------|
| read/32bits/src.psd | 25.00% | ❌ FAILED | [<img src="images/read_32bits_src_toplevel.png" width="100">](images/read_32bits_src_toplevel.png) | [<img src="images/read_32bits_src_view.png" width="100">](images/read_32bits_src_view.png) | [<img src="images/read_32bits_src_diff.png" width="100">](images/read_32bits_src_diff.png) |
| read/cmyk/src.psd | 67.53% | ⚠️ LOW | [<img src="images/read_cmyk_src_toplevel.png" width="100">](images/read_cmyk_src_toplevel.png) | [<img src="images/read_cmyk_src_view.png" width="100">](images/read_cmyk_src_view.png) | [<img src="images/read_cmyk_src_diff.png" width="100">](images/read_cmyk_src_diff.png) |
| read/16bits/src.psd | 68.38% | ⚠️ LOW | [<img src="images/read_16bits_src_toplevel.png" width="100">](images/read_16bits_src_toplevel.png) | [<img src="images/read_16bits_src_view.png" width="100">](images/read_16bits_src_view.png) | [<img src="images/read_16bits_src_diff.png" width="100">](images/read_16bits_src_diff.png) |
| read/alpha-composite/src.psd | 75.65% | ⚠️ LOW | [<img src="images/read_alpha-composite_src_toplevel.png" width="100">](images/read_alpha-composite_src_toplevel.png) | [<img src="images/read_alpha-composite_src_view.png" width="100">](images/read_alpha-composite_src_view.png) | [<img src="images/read_alpha-composite_src_diff.png" width="100">](images/read_alpha-composite_src_diff.png) |
| read/bitmap/src.psd | 85.55% | ⚠️ LOW | [<img src="images/read_bitmap_src_toplevel.png" width="100">](images/read_bitmap_src_toplevel.png) | [<img src="images/read_bitmap_src_view.png" width="100">](images/read_bitmap_src_view.png) | [<img src="images/read_bitmap_src_diff.png" width="100">](images/read_bitmap_src_diff.png) |
| read/animation-timeline/src.psd | 90.39% | ✅ GOOD | [<img src="images/read_animation-timeline_src_toplevel.png" width="100">](images/read_animation-timeline_src_toplevel.png) | [<img src="images/read_animation-timeline_src_view.png" width="100">](images/read_animation-timeline_src_view.png) | [<img src="images/read_animation-timeline_src_diff.png" width="100">](images/read_animation-timeline_src_diff.png) |
| read/animation-frame/src.psd | 90.90% | ✅ GOOD | [<img src="images/read_animation-frame_src_toplevel.png" width="100">](images/read_animation-frame_src_toplevel.png) | [<img src="images/read_animation-frame_src_view.png" width="100">](images/read_animation-frame_src_view.png) | [<img src="images/read_animation-frame_src_diff.png" width="100">](images/read_animation-frame_src_diff.png) |
| read/alias/src.psd | 97.09% | ✅ GOOD | [<img src="images/read_alias_src_toplevel.png" width="100">](images/read_alias_src_toplevel.png) | [<img src="images/read_alias_src_view.png" width="100">](images/read_alias_src_view.png) | [<img src="images/read_alias_src_diff.png" width="100">](images/read_alias_src_diff.png) |
| read/animation-offset/src.psd | 98.71% | ✅ GOOD | [<img src="images/read_animation-offset_src_toplevel.png" width="100">](images/read_animation-offset_src_toplevel.png) | [<img src="images/read_animation-offset_src_view.png" width="100">](images/read_animation-offset_src_view.png) | [<img src="images/read_animation-offset_src_diff.png" width="100">](images/read_animation-offset_src_diff.png) |
| read/blend-mode/src.psd | 98.76% | ✅ GOOD | [<img src="images/read_blend-mode_src_toplevel.png" width="100">](images/read_blend-mode_src_toplevel.png) | [<img src="images/read_blend-mode_src_view.png" width="100">](images/read_blend-mode_src_view.png) | [<img src="images/read_blend-mode_src_diff.png" width="100">](images/read_blend-mode_src_diff.png) |
| read/animation-effects/src.psd | 99.06% | ✅ PERFECT | [<img src="images/read_animation-effects_src_toplevel.png" width="100">](images/read_animation-effects_src_toplevel.png) | [<img src="images/read_animation-effects_src_view.png" width="100">](images/read_animation-effects_src_view.png) | [<img src="images/read_animation-effects_src_diff.png" width="100">](images/read_animation-effects_src_diff.png) |
| read/adjustment-layers/src.psd | 99.37% | ✅ PERFECT | [<img src="images/read_adjustment-layers_src_toplevel.png" width="100">](images/read_adjustment-layers_src_toplevel.png) | [<img src="images/read_adjustment-layers_src_view.png" width="100">](images/read_adjustment-layers_src_view.png) | [<img src="images/read_adjustment-layers_src_diff.png" width="100">](images/read_adjustment-layers_src_diff.png) |
| read/bitmap-rle/src.psd | 99.53% | ✅ PERFECT | [<img src="images/read_bitmap-rle_src_toplevel.png" width="100">](images/read_bitmap-rle_src_toplevel.png) | [<img src="images/read_bitmap-rle_src_view.png" width="100">](images/read_bitmap-rle_src_view.png) | [<img src="images/read_bitmap-rle_src_diff.png" width="100">](images/read_bitmap-rle_src_diff.png) |
| read/artboards/src.psd | 99.86% | ✅ PERFECT | [<img src="images/read_artboards_src_toplevel.png" width="100">](images/read_artboards_src_toplevel.png) | [<img src="images/read_artboards_src_view.png" width="100">](images/read_artboards_src_view.png) | [<img src="images/read_artboards_src_diff.png" width="100">](images/read_artboards_src_diff.png) |
| layer-mask/src.psd | 100.00% | ✅ PERFECT | [<img src="images/layer-mask_src_toplevel.png" width="100">](images/layer-mask_src_toplevel.png) | [<img src="images/layer-mask_src_view.png" width="100">](images/layer-mask_src_view.png) | [<img src="images/layer-mask_src_diff.png" width="100">](images/layer-mask_src_diff.png) |
| read/background/src.psd | 100.00% | ✅ PERFECT | [<img src="images/read_background_src_toplevel.png" width="100">](images/read_background_src_toplevel.png) | [<img src="images/read_background_src_view.png" width="100">](images/read_background_src_view.png) | [<img src="images/read_background_src_diff.png" width="100">](images/read_background_src_diff.png) |
| read/cat/src.psd | 100.00% | ✅ PERFECT | [<img src="images/read_cat_src_toplevel.png" width="100">](images/read_cat_src_toplevel.png) | [<img src="images/read_cat_src_view.png" width="100">](images/read_cat_src_view.png) | [<img src="images/read_cat_src_diff.png" width="100">](images/read_cat_src_diff.png) |
| read/count/src.psd | 100.00% | ✅ PERFECT | [<img src="images/read_count_src_toplevel.png" width="100">](images/read_count_src_toplevel.png) | [<img src="images/read_count_src_view.png" width="100">](images/read_count_src_view.png) | [<img src="images/read_count_src_diff.png" width="100">](images/read_count_src_diff.png) |