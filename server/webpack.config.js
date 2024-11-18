const path = require('path');
const nodeExternals = require('webpack-node-externals');

module.exports = {
  entry: './index.mjs',
  target: 'node',
  mode: 'production',
  experiments: {
    outputModule: true,
  },
  externals: [nodeExternals()],
  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: 'index.js',
    libraryTarget: 'module',
    chunkFormat: 'module',
  },
  module: {
    rules: [
      {
        test: /\.(mjs|js)$/,
        exclude: /node_modules/,
        use: {
          loader: 'babel-loader',
          options: {
            presets: ['@babel/preset-env'],
            targets: { node: '20' }
          }
        }
      }
    ]
  },
  resolve: {
    extensions: ['.js', '.mjs']
  }
};