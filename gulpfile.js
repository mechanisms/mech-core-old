var pkg = require('./package.json'); // Changed this? Need to re-run gulp to reload the
var gulp = require('gulp');
var gutil = require('gulp-util');
var coffee = require('gulp-coffee');
var header = require('gulp-header');
var concat = require('gulp-concat');
var uglify = require('gulp-uglify');
var replace = require('gulp-replace');
var rename = require('gulp-rename');
var mocha = require('gulp-mocha');
var mochaPhantom = require('gulp-mocha-phantomjs');
var browserify = require('browserify');
var transform = require('vinyl-transform');
var run = require('gulp-run');

var source = pkg.source;
var libName = pkg.name;
var libFileName = pkg.name + '.c';
var libSubName = pkg.namesub;
var libMain = pkg.main;

var banner = function(bundled) {
	return [
		'// ' + libFileName,
		'// version: ' + pkg.version,
		'// author: ' + pkg.author,
		'// license: ' + pkg.license
	].join('\n') + '\n';
};

gulp.task('default', ['build', 'mocha', 'watch-mocha']);

gulp.task('mocha', ['build'], function() {
	return gulp.src(['tests/*test.js'], {
			read: false
		})
		.pipe(mocha({
			reporter: 'spec'
		}))
		.on('error', gutil.log);
});

gulp.task('watch-mocha', function() {
	return gulp.watch(['src/**', 'tests/**'], ['mocha']);
});

gulp.task('build', function() {
	// Single entry point to browserify
	var browserified = transform(function(filename) {
		return browserify()
			.require(libMain, {
				expose: libName
			})
			.bundle();
	});

	return gulp.src(source) // list of .c files we will concat
		.pipe(concat(libFileName)) // concat into pkg.name + '.c'
		.pipe(header(banner())) // add header (your name, etc.)
		.pipe(replace('{{VERSION}}', // update version tag in code
			pkg.version))
		.pipe(replace('{{NAMESUB}}', // update namesub tag in code
			libSubName))
		.pipe(gulp.dest('dist')) // dump pkg.name + '.c'
		.pipe(run('gcc -o ' + libName + '.dbg ./dist/' + libFileName).exec())
		.pipe(run('gcc -o ' + libName + ' -D NDEBUG ./dist/' + libFileName).exec())
		// .pipe(run('./' + libName))
		.on('error', gutil.log); // log any errors
});

