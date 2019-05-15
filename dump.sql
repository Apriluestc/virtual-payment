-- MySQL dump 10.13  Distrib 5.7.26, for Linux (x86_64)
--
-- Host: localhost    Database: public
-- ------------------------------------------------------
-- Server version	5.7.26

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `txproj_buyer`
--

DROP TABLE IF EXISTS `txproj_buyer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `txproj_buyer` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(32) NOT NULL,
  `password` varchar(32) NOT NULL,
  `password_pay` varchar(32) NOT NULL,
  `card` varchar(32) NOT NULL,
  `balance` bigint(20) NOT NULL,
  `token` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=1000005 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `txproj_buyer`
--

LOCK TABLES `txproj_buyer` WRITE;
/*!40000 ALTER TABLE `txproj_buyer` DISABLE KEYS */;
INSERT INTO `txproj_buyer` VALUES (1000001,'simple','123456','654321','622202 4602079853882',123456,NULL),(1000002,'jinanyuan','jinanyuan','jinanyuan','622202 1608089953883',1000,NULL),(1000003,'rich','rich','richer','6217003320116720498',100000000,NULL),(1000004,'poor','poor','poorer','6217013320216530493',100,NULL);
/*!40000 ALTER TABLE `txproj_buyer` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `txproj_deal`
--

DROP TABLE IF EXISTS `txproj_deal`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `txproj_deal` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `buyer_id` int(11) NOT NULL,
  `seller_id` int(11) NOT NULL,
  `amount` bigint(20) NOT NULL,
  `refund` bigint(20) NOT NULL,
  `create_time` datetime NOT NULL,
  `status` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1000000013 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `txproj_deal`
--

LOCK TABLES `txproj_deal` WRITE;
/*!40000 ALTER TABLE `txproj_deal` DISABLE KEYS */;
INSERT INTO `txproj_deal` VALUES (1000000001,1000001,1002,120000,0,'2014-07-24 09:30:00',2),(1000000002,1000003,1002,13300,300,'2014-07-28 09:00:00',3),(1000000003,1000004,1002,88000,0,'2014-08-01 10:50:00',1),(1000000004,1000001,1002,1400,0,'2014-08-08 18:08:00',1),(1000000005,1000003,1002,3500,0,'2014-08-08 19:01:00',1),(1000000006,1000004,1002,80100,0,'2014-08-09 14:25:00',2),(1000000007,1000001,1003,1400,0,'2014-08-09 14:30:00',2),(1000000008,1000002,1001,38300,0,'2014-08-10 19:30:00',1),(1000000009,1000001,1003,1800,100,'2014-08-11 09:00:00',3),(1000000010,1000003,1002,3700,0,'2014-08-12 09:20:00',1),(1000000011,1000003,1004,71300,0,'2014-08-13 19:30:00',2),(1000000012,1000003,1004,12300,0,'2014-08-14 10:30:00',1);
/*!40000 ALTER TABLE `txproj_deal` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `txproj_pay`
--

DROP TABLE IF EXISTS `txproj_pay`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `txproj_pay` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `deal_id` int(11) DEFAULT NULL,
  `buyer_id` int(11) NOT NULL,
  `seller_id` int(11) NOT NULL,
  `amount` varchar(32) NOT NULL,
  `pay_time` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1000000007 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `txproj_pay`
--

LOCK TABLES `txproj_pay` WRITE;
/*!40000 ALTER TABLE `txproj_pay` DISABLE KEYS */;
INSERT INTO `txproj_pay` VALUES (1000000001,1000000001,1000001,1002,'120000','2014-07-24 09:35:00'),(1000000002,1000000002,1000003,1002,'13300','2014-07-28 09:10:00'),(1000000003,1000000006,1000004,1002,'3400','2014-08-09 19:10:00'),(1000000004,1000000007,1000001,1003,'1400','2014-08-09 19:40:00'),(1000000005,1000000009,1000001,1003,'1800','2014-08-11 11:10:00'),(1000000006,1000000011,1000003,1004,'71300','2014-08-13 21:10:00');
/*!40000 ALTER TABLE `txproj_pay` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `txproj_refund`
--

DROP TABLE IF EXISTS `txproj_refund`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `txproj_refund` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `deal_id` int(11) NOT NULL,
  `buyer_id` int(11) NOT NULL,
  `seller_id` int(11) NOT NULL,
  `amount` varchar(32) NOT NULL,
  `refund_time` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1000000003 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `txproj_refund`
--

LOCK TABLES `txproj_refund` WRITE;
/*!40000 ALTER TABLE `txproj_refund` DISABLE KEYS */;
INSERT INTO `txproj_refund` VALUES (1000000001,1000000002,1000003,1002,'300','2014-07-29 12:00:00'),(1000000002,1000000009,1000001,1003,'100','2014-08-11 11:10:00');
/*!40000 ALTER TABLE `txproj_refund` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `txproj_seller`
--

DROP TABLE IF EXISTS `txproj_seller`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `txproj_seller` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(32) NOT NULL,
  `password` varchar(32) NOT NULL,
  `card` varchar(32) NOT NULL,
  `balance` bigint(20) NOT NULL,
  `token` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=1005 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `txproj_seller`
--

LOCK TABLES `txproj_seller` WRITE;
/*!40000 ALTER TABLE `txproj_seller` DISABLE KEYS */;
INSERT INTO `txproj_seller` VALUES (1001,'simple','123456','622202 4602079853882',123456,NULL),(1002,'jinanyuan','jinanyuan','622202 1608089953883',1000,NULL),(1003,'rich','rich','6217003320116720498',100000000,NULL),(1004,'poor','poor','6217013320216530493',100,NULL);
/*!40000 ALTER TABLE `txproj_seller` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2019-05-15 12:19:11
