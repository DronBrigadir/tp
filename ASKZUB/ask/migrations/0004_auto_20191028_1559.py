# Generated by Django 2.2.6 on 2019-10-28 12:59

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('ask', '0003_auto_20191028_1432'),
    ]

    operations = [
        migrations.AlterField(
            model_name='author',
            name='avatar',
            field=models.ImageField(default='images/avatar.svg', upload_to='images/'),
        ),
    ]
