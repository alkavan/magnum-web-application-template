const path = require('path');
const webpack = require('webpack');

// WEBPACK PLUGINS
const HtmlWebpackPlugin = require('html-webpack-plugin');
const CopyWebpackPlugin = require('copy-webpack-plugin');

module.exports = {
    entry: {
        main: path.resolve(__dirname, "src/main.js"),
    },
    watch: true,
    mode: 'development',
    output: {
        path: path.join(__dirname, "build"),
        filename: '[name].bundle.js',
        chunkFilename: '[name].bundle.js'
    },
    devServer: {
        contentBase: path.join(__dirname, "build"),
        compress: true,
        port: 9000
    },
    devtool: 'inline-source-map',
    plugins: [
        new HtmlWebpackPlugin({
            filename: path.resolve(__dirname, 'build/index.html'),
            template: path.resolve(__dirname, 'dist/MyApplication.html')
        }),
        new CopyWebpackPlugin([{
            from: 'dist/EmscriptenApplication.js',
            to:   path.join(__dirname, 'build/EmscriptenApplication.js')
        }]),
        new CopyWebpackPlugin([{
            from: 'dist/MyApplication.js',
            to:   path.join(__dirname, 'build/MyApplication.js')
        }]),
        new CopyWebpackPlugin([{
            from: 'dist/MyApplication.wasm',
            to:   path.join(__dirname, 'build/MyApplication.wasm')
        }]),
        new CopyWebpackPlugin([{
            from: 'src/WebApplication.css',
            to:   path.join(__dirname, 'build/WebApplication.css')
        }]),
        new webpack.optimize.SplitChunksPlugin({})
    ],
    node: {
        fs: 'empty'
    }
};