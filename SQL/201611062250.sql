-- phpMyAdmin SQL Dump
-- version 4.6.0
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Dec 19, 2016 at 01:18 PM
-- Server version: 5.6.30
-- PHP Version: 5.6.20-pl0-gentoo

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `zpijlb`
--

-- --------------------------------------------------------

--
-- Table structure for table `application`
--

-- --------------------------------------------------------

--
-- Table structure for table `icu_device`
--

CREATE TABLE `icu_device` (
  `id` varchar(4) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `icu_device_configuration`
--

CREATE TABLE `icu_device_configuration` (
  `device_id` varchar(4) NOT NULL,
  `target_device_id` varchar(4) NOT NULL,
  `color` varchar(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `icu_queue`
--

CREATE TABLE `icu_queue` (
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `target_device_id` varchar(4) NOT NULL,
  `device_id` varchar(4) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Indexes for table `icu_device`
--
ALTER TABLE `icu_device`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `icu_device_configuration`
--
ALTER TABLE `icu_device_configuration`
  ADD PRIMARY KEY (`target_device_id`,`device_id`),
  ADD KEY `fk_queue_device_idx` (`device_id`),
  ADD KEY `fk_queue_device1_idx` (`target_device_id`);

--
-- Indexes for table `icu_queue`
--
ALTER TABLE `icu_queue`
  ADD PRIMARY KEY (`timestamp`,`target_device_id`,`device_id`),
  ADD KEY `fk_queue_device_configuration1_idx` (`target_device_id`,`device_id`);

--
-- Constraints for dumped tables
--

--
-- Constraints for table `icu_device_configuration`
--
ALTER TABLE `icu_device_configuration`
  ADD CONSTRAINT `fk_queue_device` FOREIGN KEY (`device_id`) REFERENCES `icu_device` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  ADD CONSTRAINT `fk_queue_device1` FOREIGN KEY (`target_device_id`) REFERENCES `icu_device` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION;

--
-- Constraints for table `icu_queue`
--
ALTER TABLE `icu_queue`
  ADD CONSTRAINT `fk_queue_device_configuration1` FOREIGN KEY (`target_device_id`,`device_id`) REFERENCES `icu_device_configuration` (`target_device_id`, `device_id`) ON DELETE NO ACTION ON UPDATE NO ACTION;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
